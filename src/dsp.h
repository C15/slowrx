#ifndef DSP_H_
#define DSP_H_

#include <mutex>
#include <thread>
#include "fftw3.h"
#include "common.h"
#include "input.h"

// moment length only affects length of global delay, I/O interval,
// and maximum window size.
#define FFT_LEN_SMALL  1024
#define FFT_LEN_BIG    2048

#define FREQ_MIN    500.0
#define FREQ_MAX   3300.0
#define FREQ_BLACK 1500.0
#define FREQ_WHITE 2300.0
#define FREQ_SYNC  1200.0

namespace window {
  Wave Hann (size_t);
  Wave Blackmann (size_t);
  Wave Rect (size_t);
  Wave Gauss (size_t);
}

class DSP {
  public:

    DSP ();

    double              calcPeakFreq    (double minf, double maxf, WindowType win_type);
    std::vector<double> calcBandPowerPerHz (const std::vector<std::vector<double>>&, WindowType wintype=WINDOW_HANN2047);
    double              calcVideoSNR    ();
    double              calcVideoLevel  (SSTVMode, bool is_adaptive=false);
    double              calcSyncPower   ();
    void                calcWindowedFFT (WindowType win_type, unsigned fft_len);

    int                 freq2bin        (double freq, int fft_len) const;
    WindowType          bestWindowFor   (SSTVMode, double SNR=99);
    void                set_fshift      (double);

    Input*              getInput        ();

  private:


    fftw_complex* fft_inbuf_;
    fftw_complex* fft_outbuf_;
    fftw_plan     fft_plan_small_;
    fftw_plan     fft_plan_big_;
    double        mag_[FFT_LEN_BIG/2+1];
    double        fshift_;
    double        next_snr_time_;
    double        SNR_;
    WindowType    sync_window_;

    std::vector<Wave> window_;

    Input*              input_;
};

Wave   convolve     (const Wave&, const Wave&, bool wrap_around=false);
Wave   deriv        (const Wave&);
Wave   peaks        (const Wave&, size_t);
Wave   derivPeaks   (const Wave&, size_t);
Wave   rms          (const Wave&, int);
Wave   upsample     (const Wave& orig, size_t factor, int kern_type);
double gaussianPeak (double y1, double y2, double y3);
double power        (fftw_complex coeff);
double complexMag   (fftw_complex coeff);

#endif
