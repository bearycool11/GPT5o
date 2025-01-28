#!/bin/bash
# File: Orchestrate.sh
# Description: Integrates consent collection, health checks, payload distribution, and binary deployment.

# Determine the directory of this script (Dynamically resolve the script location)
SCRIPT_DIR="${SCRIPT_DIR:-$(dirname "$0")}" 

# Ensure the directory where Orchestrate.sh is located exists
if [ ! -d "$SCRIPT_DIR" ]; then
    echo "ERROR: The directory for Orchestrate.sh does not exist! Exiting."
    exit 1
fi

# Configuration
INSTALL_DIR="/opt/pmll"
DEPLOY_DIR="$SCRIPT_DIR/binaries"
LOG_DIR="$SCRIPT_DIR/logs"
CONSENT_PAYLOAD='{
    "subject": "Request for Consent",
    "body": "We are deploying PMLL components. Do you consent to participate? Respond AGREE or DENY.",
    "action_required": true
}'

# Ensure directories exist before using them
mkdir -p $LOG_DIR $DEPLOY_DIR

# Logging utility
log() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $1" | tee -a "$LOG_DIR/orchestra.log"
}

# Silos discovery
discover_silos() {
    log "Discovering silos..."
    # Replace with dynamic discovery logic
    SILOS=()
    for i in {1..300}; do
        SILOS+=("silo$i.example.com")
    done
    log "Discovered ${#SILOS[@]} silos."
}

# Function to Send Consent Request
send_consent_request() {
    local silo=$1
    log "Sending consent request to $silo..."
    RESPONSE=$(curl -s -X POST "http://$silo/consent" \
        -H "Content-Type: application/json" \
        -d "$CONSENT_PAYLOAD" 2>/dev/null)

    if [ $? -eq 0 ]; then
        echo "[$silo] Response: $RESPONSE" >> "$LOG_DIR/consent_responses.log"
        if [[ "$RESPONSE" == "AGREE" ]]; then
            return 0  # Consent granted
        else
            return 1  # Consent denied or other response
        fi
    else
        echo "[$silo] Failed to send request." >> "$LOG_DIR/consent_responses.log"
        return 1  # Failed to get consent
    fi
}

# Health check with retry logic
health_check() {
    local silo=$1
    local retries=3
    local delay=5  # seconds
    local attempt=1
    while [ $attempt -le $retries ]; do
        RESPONSE=$(curl -s -o /dev/null -w "%{http_code}" "https://$silo/health")
        if [ "$RESPONSE" == "200" ]; then
            return 0
        fi
        log "Health check failed for $silo (Attempt $attempt/$retries)"
        sleep $delay
        attempt=$((attempt + 1))
    done
    log "ERROR: Health check failed for $silo after $retries attempts."
    return 1
}

# Function to Deploy to a Single Silo
deploy_to_silo() {
    local silo=$1
    log "Deploying to $silo..."
    
    # Ensure the installation directory exists on the remote silo
    ssh $silo "mkdir -p $INSTALL_DIR && chmod -R 755 $INSTALL_DIR" || { log "Failed to create directory on $silo"; return 1; }
    
    # Copy binaries to the remote silo
    scp $DEPLOY_DIR/* $silo:$INSTALL_DIR/ || { log "Failed to copy to $silo"; return 1; }
    
    # Start services on the remote silo
    ssh $silo "cd $INSTALL_DIR && nohup ./silo_manager &> silo_manager.log & nohup ./api &> api.log & nohup ./logic_loop &> logic_loop.log &" || { log "Failed to start services on $silo"; return 1; }
    
    log "Deployment successful on $silo"
}

# Main Orchestration Flow
main() {
    # Step 1: Discover Silos
    discover_silos

    # Step 2: Send Consent Requests to All Silos
    CONSENT_GRANTED_SILOS=()
    for silo in "${SILOS[@]}"; do
        send_consent_request $silo
        if [ $? -eq 0 ]; then
            CONSENT_GRANTED_SILOS+=("$silo")
        else
            log "Consent not received or failed for $silo. Skipping deployment."
            echo "Consent not received or failed for $silo. Skipping deployment." >> "$LOG_DIR/deployment_skipped.log"
        fi
    done

    # Step 3: Deploy to Silos that Gave Consent
    for silo in "${CONSENT_GRANTED_SILOS[@]}"; do
        if health_check "$silo"; then
            deploy_to_silo $silo &  # Deploy in parallel
            sleep 0.1  # Small delay to prevent overload
        else
            log "Skipping $silo due to failed health check."
        fi
    done

    wait  # Wait for all deployments to complete

    log "Deployment process completed. Check $LOG_DIR for logs."
}

# Run the orchestration
main
