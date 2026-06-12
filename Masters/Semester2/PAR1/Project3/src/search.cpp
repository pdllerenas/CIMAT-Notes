#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>

#include "search.h"
#include "rand.h"

int map_to_midi_pitch(double value, double min_val, double max_val)
{
  int min_midi = 60; // C4 (Middle C)
  int max_midi = 84; // C6 (Two octaves higher)

  // Scale the continuous value to a percentage (0.0 to 1.0)
  double percent = (value - min_val) / (max_val - min_val);

  // Map to the MIDI integer range
  return min_midi + static_cast<int>(round(percent * (max_midi - min_midi)));
}

Point HarmonicSearch(Func f, HParams hp)
{
  std::vector<std::vector<double>> HM(hp.HS_size, std::vector<double>(hp.ndim));
  std::vector<double> HMBest(hp.HS_size);

  // thread-safe (see implementation)
  Random r;
#ifdef VERBOSE
  if (omp_get_max_threads() == 1)
  {
    std::cout << "Iteration,";
    for (int i = 0; i < hp.ndim; i++)
    {
      std::cout << "x" << i + 1 << ',';
    }
    std::cout << "ObjectiveValue,";
    for (int i = 0; i < hp.ndim; i++)
    {
      std::cout << "Note" << i + 1 << "_MIDI" << (i < hp.ndim - 1 ? "," : "");
    }
    std::cout << std::endl;
  }
#endif

#pragma omp parallel for default(none) shared(hp, HM, HMBest, r, f)
  for (int i = 0; i < hp.HS_size; ++i)
  {
    for (int j = 0; j < hp.ndim; j++)
    {
      HM[i][j] = r.GetRand(hp.lower_bound[j], hp.upper_bound[j]);
    }
    HMBest[i] = f(HM[i]);
  }

#pragma omp parallel default(none) shared(hp, HM, HMBest, r, f, std::cout)
  {
    std::vector<double> new_x(hp.ndim);

#pragma omp for
    for (int k = 0; k < hp.MaxAttempt; k++)
    {
      for (int j = 0; j < hp.ndim; ++j)
      {
        if (r.GetRand() >= hp.HMacceptRate)
        {
          // randomized search
          new_x[j] = r.GetRand(hp.lower_bound[j], hp.upper_bound[j]);
        }
        else
        {
          // harmony memory accepting rate
          int random_hm_index = floor(hp.HS_size * r.GetRand());
          new_x[j] = HM[random_hm_index][j];

          if (r.GetRand() <= hp.PArate)
          {
            // pitch adjusting
            double pa = (hp.upper_bound[j] - hp.lower_bound[j]) / hp.pa_range[j];
            new_x[j] = new_x[j] + pa * (r.GetRand() - 0.5);
          }
        }
      }

      // evaluate the newly generated candidate solution
      double fbest = f(new_x);
      int HSmaxNum = 0;
      double HSmax = HMBest[0];

      // Find the worst current harmony
      for (int i = 1; i < hp.HS_size; ++i)
      {
        if (HMBest[i] > HSmax)
        {
          HSmax = HMBest[i];
          HSmaxNum = i;
        }
      }

#pragma omp critical
      {
        if (fbest < HSmax)
        {
          HM[HSmaxNum] = new_x;
          HMBest[HSmaxNum] = fbest;
#ifdef VERBOSE
          if (omp_get_num_threads() == 1)
          {
            std::cout << k << ",";
            for (int i = 0; i < hp.ndim; i++)
            {
              std::cout << new_x[i] << ',';
            }

            std::cout << fbest << ",";

            for (int i = 0; i < hp.ndim; i++)
            {
              int note = map_to_midi_pitch(new_x[i], hp.lower_bound[i], hp.upper_bound[i]);
              std::cout << note << (i < hp.ndim - 1 ? "," : "");
            }
            std::cout << '\n';
          }
#endif
        }
      }
    }
  }

  // find best harmony to return
  int best_idx = 0;
  double overall_best = HMBest[0];
  for (int i = 1; i < hp.HS_size; ++i)
  {
    if (HMBest[i] < overall_best)
    {
      overall_best = HMBest[i];
      best_idx = i;
    }
  }

  return HM[best_idx];
}