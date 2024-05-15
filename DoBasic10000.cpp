#include <MPerf/Core.hpp>
#include <MPerf/Tracers/LinuxPerf.hpp>

static const int N = 10000;
static int arr[N];

int main(int argc, char *argv[]) {
  auto tracer = MPerf::Tracers::LinuxPerf::Tracer();
  auto measure = tracer.MakeMeasure(MPerf::HLMeasureType::HWInstructions);
  uint64_t start, end;

  measure->DoMeasure();
  start = measure->GetJSON()["hw_instructions"];
  for (int i = 0; i < N; i++) {
    if (i % 2 == 0) {
      arr[i] += i;
    } else {
      arr[i] -= i;
    }
  }
  measure->DoMeasure();
  end = measure->GetJSON()["hw_instructions"];
  printf("%lu\n", end - start);
}
