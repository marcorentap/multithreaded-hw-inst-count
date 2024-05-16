#include <MPerf/Core.hpp>
#include <MPerf/Tracers/LinuxPerf.hpp>
#include <omp.h>

static const int N = 1000;
static int arr[N];

int main(int argc, char *argv[]) {
  auto tracer = MPerf::Tracers::LinuxPerf::Tracer();
  auto measure = tracer.MakeMeasure(MPerf::HLMeasureType::HWInstructions);
  uint64_t start, end;

  measure->DoMeasure();
  start = measure->GetJSON()["hw_instructions"];
  #pragma omp parallel for
  for (int i = 0; i < N; i++) {
    int x = omp_get_thread_num();
    if (i % 2 == 0) {
      arr[i] = 1;
    } else {
      arr[i] = -1;
    }

    arr[i] *= i;
  }
  measure->DoMeasure();
  end = measure->GetJSON()["hw_instructions"];
  printf("%lu\n", end - start);
}
