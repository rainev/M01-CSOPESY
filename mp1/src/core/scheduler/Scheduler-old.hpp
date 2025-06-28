#pragma once
#include "core/common/imports/_all.hpp"
#include "core/process/Process.hpp"
#include "core/instruction/InstructionInterpreter.hpp" // IMPORTANT: must be included before Process!!!
#include "types.hpp"

namespace csopesy {
  class Scheduler {
    using Config = SchedulerConfig;
    using Cores = vector<Ref<Process>>;
    using Cycles = vector<int>;
    using ProcList = vector<Process>;
    using ProcRefList = vector<ref<const Process>>;
    using ProcQueue = queue<ref<Process>>;

    ProcList processes;
    ProcQueue ready_queue;
    ProcList finished_processes;
    Cores cores;
    Config config;
    Cycles quantum_cycle;
    // mutable mutex queue_mutex;
    // CPUWorkerPool worker_pool;
    
  public:

    void set_config(Config new_config) {
      config = move(new_config);
      cores.resize(config.num_cpu);
      quantum_cycle.resize(config.num_cpu, 0); 
      processes.reserve(100000);
    }

    // void start_workers() {
    //   worker_pool.start_all(this, config.num_cpu);
    // }

    // void stop_workers() {
    //   worker_pool.stop_all();
    // }

    void add_process(Process process) {
      processes.push_back(move(process));
      auto proc_ref = ref(processes.back());

      for (int i = 0; i < cores.size(); ++i) {
        auto& core = cores[i];
        if (!core || core->get().get_program().is_finished()) {
          proc_ref.get().set_core(i); // 👉 assign core ID
          core = proc_ref;
          return;
        }
      }

      ready_queue.push(proc_ref);
    }


    // void run_single_core(int core_id) {
    //   lock_guard<mutex> lock(queue_mutex);

    //   auto& core = cores[core_id];

    //   if (core && !core->get().get_program().is_finished()) {
    //     auto& proc = core->get();
    //     proc.get_program().step();
    //     proc.tick();

    //     if (config.scheduler == "rr") {
    //       --quantum_cycle[core_id];
    //       if (quantum_cycle[core_id] <= 0 && !proc.get_program().is_finished()) {
    //         proc.set_core(-1);
    //         ready_queue.push(ref(proc));
    //         core.reset();
    //       }
    //     }
    //   }

    //   if (core && core->get().get_program().is_finished()) {
    //     auto& proc = core->get();
    //     proc.set_core(-1);
    //     finished_processes.push_back(proc);
    //     core.reset();
    //   }

    //   if (!core && !ready_queue.empty()) {
    //     auto proc_ref = ready_queue.front();
    //     ready_queue.pop();
    //     proc_ref.get().set_core(core_id);
    //     core = proc_ref;

    //     if (config.scheduler == "rr")
    //       quantum_cycle[core_id] = config.quantum_cycles;
    //   }
    // }

    void tick() {
      if (config.scheduler == "rr") 
        tick_rr();
      else 
        tick_fcfs();
    }

    void tick_fcfs() {
      // Step 1: Execute all assigned processes
      for (auto& core: cores) {
        if (core && !core->get().get_program().is_finished()) {
          auto& proc = core->get();
          proc.get_program().step();
          proc.tick();
        }
      }

      // Step 2: Move finished processes to the finished ProcList
      for (auto& core: cores) {
        if (core && core->get().get_program().is_finished()) {
          auto& proc = core->get();
          proc.set_core(-1);
          finished_processes.push_back(proc);
          core.reset();
        }
      }

      // Step 3: Assign ready ProcQueue to available cores
      for (int i = 0; i < cores.size(); ++i) {
        auto& core = cores[i];
        if (!core && !ready_queue.empty()) {
          auto proc_ref = ready_queue.front();
          ready_queue.pop();

          proc_ref.get().set_core(i);
          core = proc_ref;
        }
      }
    }

    // ROUND ROBIN
    void tick_rr() {
      cout << "Scheduler tick (RR)..." << endl;

      cout << "Running processes: " << get_running_processes().size() << endl;
      // Step 1: Execute the processes
      for (int i = 0; i < cores.size(); ++i) {
        auto& core = cores[i];
        if (core && !core->get().get_program().is_finished()) {
          auto& proc = core->get();

          proc.get_program().step();
          proc.tick();

          --quantum_cycle[i];

          // If quantum expired and still not finished → rotate back
          if (quantum_cycle[i] <= 0 && !proc.get_program().is_finished()) {
            proc.set_core(-1);
            ready_queue.push(ref(proc));
            core.reset();
          }
        }
      }

      // Step 2: Move finished processes
      for (int i = 0; i < cores.size(); ++i) {
        auto& core = cores[i];
        if (core && core->get().get_program().is_finished()) {
          auto& proc = core->get();
          proc.set_core(-1);
          finished_processes.push_back(proc);
          core.reset();
        }
      }

      // Step 3: Assign new processes from ready ProcQueue
      for (int i = 0; i < cores.size(); ++i) {
        auto& core = cores[i];
        if (!core && !ready_queue.empty()) {
          auto proc_ref = ready_queue.front();
          ready_queue.pop();
          proc_ref.get().set_core(i);
          quantum_cycle[i] = config.quantum_cycles;
          core = proc_ref;
        }
      }
    }

    ProcRefList get_running_processes() const {
      auto running = ProcRefList();
      for (const auto& core: cores) {
        if (!core) continue;

        const auto& proc = core->get();
        if (!proc.get_program().is_finished())
          running.push_back(cref(proc));
      }
      return running;
    }

    const ProcList& get_finished_processes() const {
      return finished_processes;
    }

    bool all_finished() const {
      return all_of(processes, [](const Process& proc) {
        return proc.get_program().is_finished();
      });
    }

    const ProcList& get_processes() const {
      return processes;
    }

    Config& get_config() { return config; }
    bool is_initialized() const { return config.initialized; }
    void mark_initialized() { config.initialized = true; }
  };
}