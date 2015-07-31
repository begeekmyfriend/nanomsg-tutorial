#!/bin/sh

BIN=bin

${BIN}/pipeline node0 &
${BIN}/pipeline node1 "Hello world!"
${BIN}/pipeline node1 "Goodbye!"

killall pipeline
