#!/bin/bash

echo "Installing dependencies..."

sudo apt update
sudo apt install -y libfuse3-dev fuse3

sudo apt install libssl-dev

echo "Done!"
