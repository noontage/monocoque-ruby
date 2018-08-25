#include <emscripten.h>
#include <mruby.h>
#include <mruby/dump.h>
#include <iostream>

#ifdef __cplusplus
extern "C"
{
#endif
  extern void alert();

  EMSCRIPTEN_KEEPALIVE void exec_irep(const uint8_t* irep, size_t length);

#ifdef __cplusplus
}
#endif

mrb_state* mrb;

int main()
{
  alert();
  EM_ASM_(
      {
        var s = Pointer_stringify($0);
        alert(s);
      },
      "TEXT");
  return 0;
}

void exec_irep(const uint8_t* irep, size_t length)
{
  mrb = mrb_open();
  mrb_load_irep(mrb, irep);
  if (mrb->exc) {
    mrb_print_backtrace(mrb);
    mrb_print_error(mrb);
  }
}