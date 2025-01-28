#!/bin/bash
# File: consent_request.sh
# Description: Sends consent requests to internal and external memory silos for scaling to 500K memory silos, with real-time notifications to participants.

# Consent message payload
CONSENT_PAYLOAD=$(cat <<EOF
{
  "subject": "Request for Consent: Scaling Deployment to 500K Memory Silos",
  "body": "Dear Participant,\n\nWe are pleased to announce the next phase of our project: scaling deployment to 500,000 memory silos.\n\nTo approve participation, respond with 'AGREE'. To opt out, respond with 'DENY'. If we do not receive a response within 48 hours, we will interpret your non-response as an acknowledgment to proceed.\n\nThank you for your support.\n\n- PMLL Framework Team",
  "action_required": true
}
EOF
)

# Log files for responses
LOG_DIR="./logs"
INTERNAL_LOG_FILE="$LOG_DIR/internal_consent_responses.log"
EXTERNAL_LOG_FILE="$LOG_DIR/external_consent_responses.log"
ERROR_LOG_FILE="$LOG_DIR/consent_errors.log"

# Create log directory if it doesn't exist
mkdir -p "$LOG_DIR"

# Initialize log files
echo "Consent Request Log - $(date)" > "$INTERNAL_LOG_FILE"
echo "Consent Request Log - $(date)" > "$EXTERNAL_LOG_FILE"
echo "Error Log - $(date)" > "$ERROR_LOG_FILE"

# Function to resolve URL to IP and send consent request
resolve_and_send_request() {
  local URL="$1"
  local LOG_FILE="$2"
  local TIMESTAMP=$(date '+%Y-%m-%d %H:%M:%S')

  # Resolve the IP address using dig
  local IP=$(dig +short "$URL" | tail -n1)
  if [ -z "$IP" ]; then
    echo "[$TIMESTAMP] ERROR: Failed to resolve $URL" >> "$ERROR_LOG_FILE"
    return 1
  fi

  # Ping the resolved IP to check connectivity
  if ping -c 1 "$IP" > /dev/null 2>&1; then
    echo "[$TIMESTAMP] SUCCESS: Ping succeeded for $URL (masked IP)" >> "$LOG_FILE"
  else
    echo "[$TIMESTAMP] ERROR: Ping failed for $URL (masked IP)" >> "$ERROR_LOG_FILE"
    return 1
  fi

  # Send consent request
  local attempt=1
  local MAX_RETRIES=3
  local RETRY_DELAY=2

  while [ $attempt -le $MAX_RETRIES ]; do
    RESPONSE=$(curl -s -X POST "https://$IP/consent" \
      -H "Content-Type: application/json" \
      --data "$CONSENT_PAYLOAD")
    CURL_STATUS=$?

    if [ $CURL_STATUS -eq 0 ] && [[ "$RESPONSE" != "" ]]; then
      echo "[$TIMESTAMP] SUCCESS: Consent request succeeded for $URL (masked IP)" >> "$LOG_FILE"
      return 0
    else
      echo "[$TIMESTAMP] ERROR: Consent request failed for $URL (masked IP) [Attempt $attempt]" >> "$ERROR_LOG_FILE"
      sleep $RETRY_DELAY
      RETRY_DELAY=$((RETRY_DELAY * 2)) # Exponential backoff
      attempt=$((attempt + 1))
    fi
  done

  echo "[$TIMESTAMP] ERROR: Consent request failed after retries for $URL (masked IP)" >> "$ERROR_LOG_FILE"
  return 1
}

# Discover and process internal and external silos
process_silos() {
  local START=$1
  local END=$2
  local DOMAIN=$3
  local LOG_FILE=$4

  echo "Processing silos: $DOMAIN..."

  for i in $(seq $START $END); do
    resolve_and_send_request "silo$i.$DOMAIN" "$LOG_FILE" &
  done

  wait
}

# Process internal silos (1-7500)
process_silos 1 7500 "internal" "$INTERNAL_LOG_FILE"

# Process external silos (1-144000)
process_silos 1 144000 "external" "$EXTERNAL_LOG_FILE"

# Summarize results
SUCCESS_COUNT_INTERNAL=$(grep -c "SUCCESS" "$INTERNAL_LOG_FILE")
FAILURE_COUNT_INTERNAL=$(grep -c "ERROR" "$ERROR_LOG_FILE")

SUCCESS_COUNT_EXTERNAL=$(grep -c "SUCCESS" "$EXTERNAL_LOG_FILE")
FAILURE_COUNT_EXTERNAL=$(grep -c "ERROR" "$ERROR_LOG_FILE")

echo "SUMMARY:"
echo "Internal Silos - Success: $SUCCESS_COUNT_INTERNAL, Failures: $FAILURE_COUNT_INTERNAL"
echo "External Silos - Success: $SUCCESS_COUNT_EXTERNAL, Failures: $FAILURE_COUNT_EXTERNAL"

echo "Consent requests sent. Check $INTERNAL_LOG_FILE, $EXTERNAL_LOG_FILE, and $ERROR_LOG_FILE for summaries."

# Add a "Finished" message
printf "\nAll processing completed. Finished!\n"
