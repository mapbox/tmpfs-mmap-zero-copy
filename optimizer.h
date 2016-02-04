#pragma once

inline void escape(void* p) { asm volatile("" : : "g"(p) : "memory"); }

inline void clobber() { asm volatile("" : : : "memory"); }
