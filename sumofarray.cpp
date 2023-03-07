#include <iostream>
#include <chrono>
#include "taskflow/taskflow.hpp"

const int ARRAY_SIZE = 1000000;

// Define the function to be executed in parallel
void add_arrays(int* a, int* b, int* c, int start, int end) {
  for (int i = start; i < end; i++) {
    c[i] = a[i] + b[i];
  }
}

int main() {
  // Create the arrays to be added together
  int* a = new int[ARRAY_SIZE];
  int* b = new int[ARRAY_SIZE];
  int* c = new int[ARRAY_SIZE];

  // Initialize the arrays with some values
  for (int i = 0; i < ARRAY_SIZE; i++) {
    a[i] = i;
    b[i] = i * 2;
  }

  // Create a taskflow object
  tf::Taskflow tf;

  // Divide the work into multiple tasks
  int num_tasks = 4;
  int chunk_size = ARRAY_SIZE / num_tasks;
  for (int i = 0; i < num_tasks; i++) {
    int start = i * chunk_size;
    int end = (i + 1) * chunk_size;
    auto task = tf.emplace([=, &a, &b, &c](){ add_arrays(a, b, c, start, end); });
    task.name("Addition task " + std::to_string(i));
  }

  // Execute the tasks in parallel
  tf::Executor executor;
  auto start_time = std::chrono::high_resolution_clock::now();
  executor.run(tf).wait();
  auto end_time = std::chrono::high_resolution_clock::now();

  // Print the result
  std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " ms" << std::endl;
  std::cout << "Result: ";
  for (int i = 0; i < 10; i++) {
    std::cout << c[i] << " ";
  }
  std::cout << std::endl;

  // Clean up
  delete[] a;
  delete[] b;
  delete[] c;

  return 0;
}
