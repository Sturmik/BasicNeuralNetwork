#pragma once

// Contains examples of neural network usage

// XOR problem solving
void XORProblemSolvingExample();

// Neural network BackgroundColor prediction training by using supervised learning.
// Allows user to choose, which foreground color (black or white) suits best to given background color, thus
// teaching neural network to do so, base on user input. Network will mark it's choice by placing circle
// above best foreground color.
void ColorPredictorExample();

// Neural network number recognition
// DrawChek - if true, will allow user to draw and check neural network guess
// otherwise will randomly pick number for network to analyze. Results of guess will be printed in the console
//
// Controls if true:
// Left mouse button to draw
// Right mouse button to let neural network gues
// Spacebar clear screen
//
// Controls if false:
// Left mouse button to change test sample
void NumberRecognitionExample(bool CanDrawCheck);