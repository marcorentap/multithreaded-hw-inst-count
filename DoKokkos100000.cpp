#include <Kokkos_Core.hpp>
#include <MPerf/Core.hpp>
#include <MPerf/Tracers/LinuxPerf.hpp>

static const int N = 100000;

int main(int argc, char *argv[]) {
  auto guard = Kokkos::ScopeGuard(argc, argv);
  auto tracer = MPerf::Tracers::LinuxPerf::Tracer();
  auto measure = tracer.MakeMeasure(MPerf::HLMeasureType::HWInstructions);
  Kokkos::View<int[N]> arr("arr");
  uint64_t start, end;

  measure->DoMeasure();
  start = measure->GetJSON()["hw_instructions"];
  Kokkos::parallel_for(
      N, KOKKOS_LAMBDA(const int &i) {
        if (i % 2 == 0) {
          arr(i) = 1;
        } else {
          arr(i) = -1;
        }

        arr(i) *= i;
      });
  measure->DoMeasure();
  end = measure->GetJSON()["hw_instructions"];
  printf("%lu\n", end - start);
}
