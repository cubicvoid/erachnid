#pragma once

#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <sys/time.h>

#include <clap/clap.h>

class ChompPlugin {
public:
   ChompPlugin(const clap_host_t *_host);

   FILE *logFile;

   void flog(const clap_host_t *host, clap_log_severity sev, const char *msg) {
      if (logFile != nullptr) {
         struct timeval tv;
         gettimeofday(&tv, nullptr);

         char buf[512];
         snprintf(buf, sizeof(buf), "(%d) %ld.%lf %s\n",
            sev, tv.tv_sec,
            static_cast<double>(tv.tv_usec) / 1000000.0,
            msg);
         fwrite(buf, 1, strlen(buf), logFile);
         fflush(logFile);
      }
   }

   clap_plugin_t                   plugin;
   const clap_host_t              *host;
   const clap_host_latency_t      *host_latency;
   const clap_host_thread_check_t *host_thread_check;
   const clap_host_state_t        *host_state;

   void(CLAP_ABI *log)(const clap_host_t *host, clap_log_severity severity, const char *msg);
   
   uint32_t latency;

   bool active;
   bool processing;
};

clap_process_status chomp_process(
   const struct clap_plugin *plugin,
   const clap_process_t     *process
);

extern const clap_plugin_params_t s_chomp_params;
extern const clap_plugin_audio_ports_t s_chomp_audio_ports;