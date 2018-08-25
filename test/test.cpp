#include <emscripten.h>
#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/dump.h>

#include <iostream>

#ifdef __cplusplus
extern "C"
{
#endif
  extern void alert();

  EMSCRIPTEN_KEEPALIVE void mqrb_exec_mruby_irep(const uint8_t* irep, size_t length);

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

  // text to ruby
  mrb_state* mrb_c = mrb_open();
  mrb_load_string(mrb_c, "p Time.now\n");
  mrb_close(mrb_c);
  return 0;
}

void mqrb_exec_mruby_irep(const uint8_t* irep, size_t length)
{
  mrb = mrb_open();
  mrb_load_irep(mrb, irep);
  if (mrb->exc) {
    mrb_print_backtrace(mrb);
    mrb_print_error(mrb);
  }
}