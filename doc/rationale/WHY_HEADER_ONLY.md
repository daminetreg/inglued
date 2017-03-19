# Why Header Only Dependencies only ?
Well the only reason why someone would ship a library precompiled and reuse it as such is either to spare space in RAM, on Disk or to reduce compilation time.

We measured on different platforms in serious benchmarks and evaluation that binary linking is in fact a barrier to spare space, nor RAM or Disk space because it prevent effective whole-program optimization to be achieved by the compiler.

Therefore we propose to keep on changing the world and **let end-users applications** of libraries decided to separate usage of their libraries in different compilation unit. But the unflexibility of a binary build by default is inacceptable.

By the way, C++1zz is standardizing modules for a Technical Specifications, and there are not any different than header only dependencies in an efficient binary serialized form.

So if you follow us in the header-only wave, we will make you ready for modules in C++1zz.
