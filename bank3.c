/* 

1. Bankas (ADT: eilė, ilgas sveikas skaičius). 

Procesas: klientai ateina į banką ir kreipiasi į bet kurį laisvą kasininką,
jei visi kasininkai užsiėmę, klientai stoja į bendrą eilę. 
Tikslas: patyrinėti, kaip tikslingiau bankui elgtis –
turėti mažiau labiau patyrusių kasininkų (jie aptarnauja klientus greičiau)
ar daugiau mažiau patyrusių kasininkų (jie aptarnauja klientus lėčiau).

Pradiniai duomenys: kasininkų skaičius, kliento aptarnavimo laikas,
kliento atvykimo tikimybė 
(galima realizacija: generuojamas atsitiktinis skaičius ir tikrinama,
ar jis tenkina tam tikrą sąlygą). 

Rezultatai: statistika kažkuriam (vartotojo nurodytam) laiko momentui,
pavyzdžiui, vidutinis ir maksimalus kliento laukimo laikas, 
vidutinis kasininko užimtumas (procentais), maksimalus eilės dydis ir pan.

*/

/*

LSP: 2110576
2 užduotis, bankas


DEADLINE: 05.04

*/

#include <stdio.h>
#include <time.h>
#include "queue.c"


#define MAX_CASHIERS_NUM 40
#define MIN_CASHIERS_NUM 5
#define MAX_CLIENT_SERVING_TIME 60
#define MIN_CLIENT_SERVING_TIME 5
#define MAX_CLIENT_ARRIVAL_PROB 100
#define MIN_CLIENT_ARRIVAL_PROB 10

#define MIN 0 

int CASHIERS_NUM;
int CLIENT_SERVING_TIME;
int CLIENT_ARRIVAL_PROB;

int enterTime(){
    int time;
    printf("Enter simulation time:\n");
    scanf("%d", &time);
    return time;
}

bool check_cashiers(Queue *busy_q){
    return busy_q->size < CASHIERS_NUM;
}

bool is_client_arriving(){
    return rand() % 100 < CLIENT_ARRIVAL_PROB;
}

void simulation(Queue *waiting_q, Queue *busy_q, int simulation_time, Queue *clients, int *q_lengths){
    Client *clients_arr = malloc(simulation_time*sizeof(Client));
    int i = -1;
    for (int time = 0; time < simulation_time; ++ time){
        if(is_client_arriving()){
            i ++;
            
            clients_arr[i].id = i;
            clients_arr[i].waiting_time = 0;
            clients_arr[i].serving_time = 0;
            clients_arr[i].is_served = false;  

            enqueue(clients, &clients_arr[i]);

            if(!check_cashiers(busy_q)){
                enqueue(waiting_q, &clients_arr[i]);

            }else if(check_cashiers(busy_q) && !is_empty(*waiting_q)){
                enqueue(waiting_q, &clients_arr[i]);
                Client *client_temp =  dequeue(waiting_q);
                enqueue(busy_q, client_temp);
                
            }else if(check_cashiers(busy_q) && is_empty(*waiting_q)){
                enqueue(busy_q, &clients_arr[i]);
            }
        
        }
        
        q_lengths[time] = waiting_q->size;

        if(!is_empty(*waiting_q)){
            for(int j = 0; j < waiting_q->size; j++){
                Client *client_temp = dequeue(waiting_q);
                client_temp->waiting_time ++;
                enqueue(waiting_q, client_temp);
            }
        }

        if(!is_empty(*busy_q)){
            for(int z = 0; z < busy_q->size; z++){
                Client *client_temp = dequeue(busy_q);
                client_temp->serving_time ++;
                if(client_temp->serving_time >= CLIENT_SERVING_TIME){
                    client_temp->is_served = true;
                    continue;
                }
                enqueue(busy_q, client_temp);
            }
        } 
    }

}


void client_statistics(Queue *clients, int *average, int *max, double *served_clients_ratio){
    int sum = 0;
    int current_max = MIN;
    int length = clients->size;
    int waiting_time;
    double served_clients = 0.0;

    for(int i = 0; i < length; i++){
        Client *client = dequeue(clients);
        waiting_time = client->waiting_time;
        sum += waiting_time;
        if(waiting_time > current_max){
            current_max = waiting_time;
        }
        if(client->is_served == true){
            ++served_clients;
        }

    }

    *served_clients_ratio = served_clients/length;
    *average = sum/length;
    *max = current_max;
}

void q_statistics(int *q_lengths, int *max_q, int *average_q, int simulation_time){
    int current_max_q = MIN;
    int sum = 0;
    int length = simulation_time;
    for (int i = 0; i < length; i++){
        sum += q_lengths[i];
        if(q_lengths[i] > current_max_q){
            current_max_q = q_lengths[i];
        }
    }

    *max_q = current_max_q;
    *average_q = sum / length;

}

void print_results(int simulation_time, int max, int average, int max_q, int average_q, double served_clients_ratio){
    printf("Simulation time: %d\n", simulation_time);
    printf("Number of cashiers: %d\n", CASHIERS_NUM);
    printf("Client serving time of one cashier: %d\n", CLIENT_SERVING_TIME);
    printf("Client arrival probability[%%]: %d\n", CLIENT_ARRIVAL_PROB);
    printf("Served clients ratio: %.2f\n", served_clients_ratio);
    printf("Client waiting time:\n- Max: %d\n- Average: %d\n", max, average);
    printf("Queue length:\n- Max: %d\n- Average: %d\n", max_q, average_q);
    printf("----------------------------------------------------\n");
}

int generate_variable(int upper, int lower){
    return (rand() % (upper - lower + 1)) + lower;
}

void compare_results(int cahsiers_num, int client_serving_time, int client_arrival_prob, int simulation_time){
    
    Queue waiting_q = queue_new();
    Queue busy_q = queue_new();
    Queue clients = queue_new();

    int *q_lengths;
    q_lengths = malloc(simulation_time*sizeof(int));
    int average, max, average_q, max_q;
    double served_clients_ratio;
    
    CASHIERS_NUM = cahsiers_num;
    CLIENT_SERVING_TIME = client_serving_time;
    CLIENT_ARRIVAL_PROB = client_arrival_prob;

    simulation(&waiting_q, &busy_q, simulation_time, &clients, q_lengths);

    client_statistics(&clients, &average, &max, &served_clients_ratio);
    q_statistics(q_lengths, &max_q, &average_q, simulation_time);
    printf("SIMULATION REZULTS:\n");
    print_results(simulation_time, max, average, max_q, average_q, served_clients_ratio);

    free(q_lengths);
    queue_clear(&waiting_q);
    queue_clear(&busy_q);
    queue_clear(&clients);

}



int main(void){

    srand(time(0));

    int simulation_time = enterTime();

    // int i = 0;
    // while (i < 10){
    //     int cashiers_num = generate_variable(MAX_CASHIERS_NUM, MIN_CASHIERS_NUM);
    //     int client_serving_time = generate_variable(MAX_CLIENT_SERVING_TIME, MIN_CLIENT_SERVING_TIME);
    //     int client_arrival_prob = generate_variable(MAX_CLIENT_ARRIVAL_PROB, MIN_CLIENT_ARRIVAL_PROB);
    //     compare_results(cashiers_num, client_serving_time, client_arrival_prob, simulation_time);
    //     i++;
    // }
    
    compare_results(20, 40, 90, simulation_time);
    // compare_results(5, 10, 90, simulation_time);

    // compare_results(10, 40, 50, simulation_time);
    // compare_results(5, 10, 50, simulation_time);

    // compare_results(10, 40, 30, simulation_time);
    // compare_results(5, 10, 30, simulation_time);




}

