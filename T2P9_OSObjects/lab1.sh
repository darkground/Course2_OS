#!/bin/bash
echo "Before unshare: $(uname -n)"
unshare -u bash lab1_hostname.sh
echo "After unshare: $(uname -n)"