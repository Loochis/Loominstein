//
// Created by loochis on 15/01/24.
//

#include <asciichis.h>
#include <mathchis.h>

#include <stdio.h>
#include <iostream>
#include <thread>

using namespace ASCIICHIS;
using namespace MATHCHIS;

asciirenderer rend;
int testCol;

#define NUM_THREADS 64

std::thread *threads;
vector *ray_dirs;

void castline(int line) {
    int i = 0;
    while (true) {
        i++;
        if (i == 100)
            return;

        //rend.setpix(i, id, testCol);
    }
}

int main () {
    rend = asciirenderer();
    threads = (std::thread*)malloc(rend.width * sizeof(std::thread));
    ray_dirs = (vector*)malloc(rend.width * sizeof(vector));

    for (int i = 0; i < rend.width; i++) {
        ray_dirs[]
    }
    //testCol = vector(1,0.5,0).col_tobytes();

    float vec1_array[] = {1, 0, -1, -3, 2, 1};
    float vec2_array[] = {2, 1, 0};

    std::vector<float> vec1_stdvec;
    std::vector<float> vec2_stdvec;

    for (int i = 0; i < 6; i++) {
        vec1_stdvec.push_back(vec1_array[i]);
    }

    for (int i = 0; i < 3; i++) {
        vec2_stdvec.push_back(vec2_array[i]);
    }

    matrix m1 = matrix(2, 3, vec1_stdvec);
    vector v1 = vector(vec2_stdvec);

    printf("m1: ");
    for (int i = 0; i < m1.elems.size(); i++) {
        printf("%f, ", m1.elems[i]);
    }
    printf("\nv1: ");
    for (int i = 0; i < v1.elems.size(); i++) {
        printf("%f, ", v1.elems[i]);
    }

    vector v3 = v1*m1;

    printf("\nv3: ");
    for (int i = 0; i < v3.elems.size(); i++) {
        printf("%f, ", v3.elems[i]);
    }

    /*for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = std::thread(castline, i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
    }

    rend.pushscreen();
     */

    exit(0);
}