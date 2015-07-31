#!/bin/sh

BIN=bin

${BIN}/bus node0 &
${BIN}/bus node1 &
${BIN}/bus node2 &
${BIN}/bus node3 &
sleep 3

killall bus
