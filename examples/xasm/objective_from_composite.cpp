#include "qcor.hpp"

const std::string src = R"(__qpu__ void ansatz(qreg q, double theta) {
  X(q[0]);
  Ry(q[1], theta);
  CX(q[1],q[0]);
})";

int main(int argc, char **argv) {

  // When manually writing kernel source strings
  // you have to specify the backend you are targeting
  qcor::set_backend("tnqvm");

  // Allocate 2 qubits
  auto q = qalloc(2);

  // Create the Deuteron Hamiltonian (Observable)
  auto H = qcor::createObservable(
      "5.907 - 2.1433 X0X1 - 2.1433 Y0Y1 + .21829 Z0 - 6.125 Z1");

  // JIT compile the ansatz
  auto ansatz = qcor::compile(src);

  // Create the ObjectiveFunction, here we want to run VQE
  // need to provide ansatz and the Observable
  auto objective = qcor::createObjectiveFunction("vqe", ansatz, H);

  // Evaluate the ObjectiveFunction at a specified set of parameters
  auto energy = (*objective)(q, .59);
  printf("vqe-energy = %f\n", energy);
}
