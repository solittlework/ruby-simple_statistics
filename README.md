# SimpleStatistics

Simple Statistics aims to provide high performance survival statistic functions for Ruby. For speed, all of the functions in this gem are natively written by C, and then wrapped into Ruby. Some of the functions are transferred from the GNU Scientific Library.

This gem is still under active development. If you have found any bugs or specifc requirements, please email haipeng3@ualberta.ca
## Installation

Add this line to your application's Gemfile:

```ruby
gem 'sstat'
```

Then install using
```ruby
    $ gem install 
```

## Usage

```ruby
# Include the library
require 'sstat'
``````
#### Basic Histogram Analysis
```ruby
# Create statistics instance
sstat_hist = SStat::Hist.new
bin = [2, 3, 5, 2, 1]
range = [0, 1, 2, 3, 4, 5]
hist_mean = sstat_hist.hist_mean(bin, range)
# Expected value for hist_mean.round(3): 2.269

hist_median = sstat_hist.hist_median(bin, range)
# Expected  value for hist_median.round(3): 2.500
hist_median.round(3) => 2.500
``````
#### Kaplan Meier estimator for survival prediction
```ruby
sstat_survival = SStat::Surv.new
# Event time for samples
time = [1, 2, 3, 4, 5, 6, 7]
# Cesnroed information
# 1 -> censored. The event time is the patient's last visit time.
# 0 -> uncensored. The event time is the patient's actual dead time.
censored = [1, 1, 0, 0, 0, 1, 0]

# Calculate the Kaplan-Meier index without extraploation
# Reference: https://en.wikipedia.org/wiki/Kaplan%E2%80%93Meier_estimator
sstat_survival.kaplan_meier(time, censored)

# Calculate Kaplan-Meier with extrapoloation based on last 3 points
sstat_instance.kaplan_meier_3p_extraploation(time, censored)

``````

## Development

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake spec` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## License

The gem is available as open source under the terms of the [MIT License](http://opensource.org/licenses/MIT).