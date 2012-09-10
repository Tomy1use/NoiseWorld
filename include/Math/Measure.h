#pragma once

template<class F>
static inline float centi(const F& cm) {return float(cm) / 100;}

template<class F>
static inline float milli(const F& mm) {return float(mm) / 1000;}
