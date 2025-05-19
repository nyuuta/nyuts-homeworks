#!/bin/bash

echo "Installing FUSE3 dependencies..."

sudo apt update
sudo apt install -y libfuse3-dev fuse3

echo "Done!"
