#ifdef BUILD_HOST
#include <common.h>
int main()
{
  auto mrb = mrb_open();
  mrb_close(mrb);
  return 0;
}
#endif