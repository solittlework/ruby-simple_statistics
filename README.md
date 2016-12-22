# SimpleStatistics

Simple Statistics aims to provide high performance survival statistic functions for Ruby. For speed, all of the functions here are natively written by C, then wrapped into Ruby. Some of the functions are transferred from the GNU Scientific Library.

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
#### Basic Histogram Analysis
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
#### Kaplan Meier index for survival analysis
```ruby
sstat_survival = SStat::Surv.new
# Event time for samples
time = [1, 2, 3, 4, 5, 6, 7]
# Cesnroed information
# 1 -> censored. The event time is the patient's last visit time.
# 0 -> uncensored. The event time is the patient's actual dead time.
censored = [1, 1, 0, 0, 0, 1, 0]

# Calculate Kaplan-Meier index without extraploation
sstat_survival.kaplan_meier(time, censored)

# Calculate Kaplan-Meier with extrapoloation based on last 3 points
sstat_instance.kaplan_meier_3p_extraploation(time, censored)

``````

## License

The gem is available as open source under the terms of the [MIT License](http://opensource.org/licenses/MIT).