#!/bin/bash

# Constants
PRESS_RELEASE_FILE="press_release.json"
ENCRYPTED_FILE="encrypted_release.bin"
LOG_FILE="orchestra.log"

# Log function
log() {
    local level=$1
    local message=$2
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] [$level] $message" | tee -a "$LOG_FILE"
}

# Check if the press release JSON file exists
if [ ! -f "$PRESS_RELEASE_FILE" ]; then
    log "ERROR" "File $PRESS_RELEASE_FILE not found!"
    exit 1
fi

# Encrypt the press release JSON file
log "INFO" "Encrypting press release..."
./encrypt_release.sh "$PRESS_RELEASE_FILE"
if [ $? -ne 0 ]; then
    log "ERROR" "Failed to encrypt the press release."
    exit 1
fi

# Verify the encrypted file exists
if [ ! -f "$ENCRYPTED_FILE" ]; then
    log "ERROR" "Encrypted file $ENCRYPTED_FILE not found after encryption step."
    exit 1
fi

# Deploy the encrypted file to memory silos
log "INFO" "Deploying encrypted press release to memory silos..."
./deploy_to_silos.sh "$ENCRYPTED_FILE"
if [ $? -ne 0 ]; then
    log "ERROR" "Deployment failed."
    exit 1
fi

# Verify deployment (optional step - add specific checks here)
log "INFO" "Verifying deployment..."
if ./verify_deployment.sh "$ENCRYPTED_FILE"; then
    log "INFO" "Deployment verified successfully."
else
    log "WARNING" "Deployment verification failed. Proceed with caution."
fi

log "INFO" "Press release deployed successfully!"
exit 0
