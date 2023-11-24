#pragma once

#include <clap/clap.h>

#include <memory>

namespace erachnid::chomp {

class Plugin;

class GUIWrapper {
 public:
  virtual ~GUIWrapper();

  static GUIWrapper *New(Plugin *plugin);

  virtual bool Create(const char *api, bool is_floating) = 0;
  virtual bool SetScale(double scale) = 0;
  virtual void Destroy() = 0;
  virtual bool GetSize(uint32_t *width, uint32_t *height) = 0;
  virtual bool CanResize() = 0;
  virtual bool AdjustSize(uint32_t *width, uint32_t *height) = 0;
  virtual bool SetSize(uint32_t width, uint32_t height) = 0;
  virtual bool SetParent(const clap_window_t *window) = 0;
  virtual bool SetTransient(const clap_window_t *window) = 0;
  virtual bool Show() = 0;
  virtual bool Hide() = 0;
};

}  // namespace erachnid::chomp
