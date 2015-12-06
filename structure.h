typedef struct jobs{
        int id, processTime, weights, duedate;
}Tjob;

typedef struct inst{
        int id, size, P_bar, P_min, P_max, S_bar, MaxWeight,
        C_max, D_bar;
        float Tau, Eta, R;
        Tjob *jobs;
        int **Distances;
}Tinstance;