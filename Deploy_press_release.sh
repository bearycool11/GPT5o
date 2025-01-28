#!/bin/bash

# Check if the press release JSON file exists
if [ ! -f "press_release.json" ]; then
    echo "Error: press_release.json not found!"
    exit 1
fi

# Encrypt the press release JSON file
echo "Encrypting press release..."
./encrypt_release.sh press_release.json
if [ $? -ne 0 ]; then
    echo "Error: Failed to encrypt the press release."
    exit 1
fi

# Deploy the encrypted file to memory silos
echo "Deploying encrypted press release..."
./deploy_to_silos.sh encrypted_release.bin
if [ $? -ne 0 ]; then
    echo "Error: Deployment failed."
    exit 1
fi

echo "Press release deployed successfully!"
