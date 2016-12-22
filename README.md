# SimpleStatistics

Simple Statistics aims to provide high performance survival statistic functions for Ruby. For better performance, all of the functions here are natively written by C, then wrapped into Ruby.

This gem is still under active development. If you have found any bugs or specifc requirements, please email haipeng3@ualberta.ca
## Installment

In Gemfile, add following:

```ruby
gem 'sstat'
```

Install simple statistics gem using:
```ruby
gem install 
```

## Usage

```ruby
# Include simple statistics
require 'sstat'
``````
#### Histogram
```ruby
# Create statistics instance
sstat_hist = SStat::Hist.new
bin = [2, 3, 5, 2, 1]
range = [0, 1, 2, 3, 4, 5]

# Calculate the histogram mean
hist_mean = sstat_hist.hist_mean(bin, range)

# Calculate the histogram median
hist_median = sstat_hist.hist_median(bin, range)

``````
#### Kaplan Meier Estimator
```ruby
sstat_survival = SStat::Surv.new
# Event time for samples
time = [1, 2, 3, 4, 5, 6, 7]

# 1 -> censored. The event time is the patient's last visit time.
# 0 -> uncensored. The event time is the patient's actual dead time.
censored = [1, 1, 0, 0, 0, 1, 0]

# Calculate Kaplan-Meier without extraploation
sstat_survival.kaplan_meier(time, censored)

# Calculate Kaplan-Meier with extrapoloation based on last 3 points
sstat_instance.kaplan_meier_3p_extraploation(time, censored)

``````

#### Log Rank Test
```ruby
# time_test_1 -> Event time array for first test
# cens_test_1 -> Censored array for first test
# time_test_2 -> Event time array for second test
# cens_test_2 -> Censored array for second test
sstat_instance.log_rank_test(time_test_1, cens_test_1, time_test_2, cens_test_2)
``````

## License

The gem is available as open source under the terms of the [MIT License](http://opensource.org/licenses/MIT).