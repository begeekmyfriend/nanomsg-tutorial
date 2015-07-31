#!/bin/sh

BIN=bin

${BIN}/reqrep node0 &
sleep 1

${BIN}/reqrep node1

killall reqrep
