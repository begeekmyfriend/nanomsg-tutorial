#!/bin/sh

BIN=bin

${BIN}/survey server &
${BIN}/survey client c0 &
${BIN}/survey client c1 &
${BIN}/survey client c2 &

sleep 1

killall survey 
