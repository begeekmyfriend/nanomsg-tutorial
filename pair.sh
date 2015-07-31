#!/bin/sh

BIN=bin

${BIN}/pair node0 &
${BIN}/pair node1 &
sleep 3

killall pair
