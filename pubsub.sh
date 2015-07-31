#!/bin/sh

BIN=bin

${BIN}/pubsub server &
sleep 1

${BIN}/pubsub client c0 &
${BIN}/pubsub client c1 &
${BIN}/pubsub client c2 &
sleep 5

killall pubsub
