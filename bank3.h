#ifndef BANK3_H
#define BANK3_H

#include <stdio.h>
#include "queue.h"
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

int enterTime();
bool check_cashiers(Queue *busy_q);
bool is_client_arriving();
void simulation(Queue *waiting_q, Queue *busy_q, int simulation_time, Client *clients, int *q_lengths);
void client_statistics(Queue *clients, int *average, int *max, double *served_clients_ratio);
void q_statistics(int *q_lengths, int *max_q, int *average_q, int simulation_time);
void print_results(int simulation_time, int max, int average, int max_q, int average_q, double served_clients_ratio);
int generate_variable(int upper, int lower);
void compare_results(int cahsiers_num, int client_serving_time, int client_arrival_prob, int simulation_time);

#endif