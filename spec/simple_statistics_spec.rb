require 'spec_helper'
require 'sstat' #simple statistics
require 'csv' #used to read file

def read_csv_by_column(csv_file, col_num)
	csv = CSV.read(csv_file, :headers=>true)
	return csv[col_num]
end

def array_to_double(array)
	float_array = []
	array.map{ |x| 
    if not x.nil?
      float_array.push(x.to_f)
    end
  }
	return float_array
end

def array_to_censored(array)
  int_array = []
  array.map{ |x|
    if not x.nil?
      int_array.push(x.to_i)
    end
  }
  return int_array
end

describe SimpleStatistics do
  it 'has a version number' do
    expect(SimpleStatistics::VERSION).not_to be nil
  end

  it 'check percentile function' do
  	sstat_instance = SStat::Surv.new
  	testing_file = Dir.pwd + '/spec/testing_data/testing_dataset_1.csv'
  	array = read_csv_by_column(testing_file, 'Median')
  	array = array_to_double(array)
  	s = array.sort
    #function of percentile_index is not as expected
    expect(sstat_instance.percentile_index(s, 0.25)).to be 1098
    expect(sstat_instance.percentile(s, 0.25)).to be 15.254
    expect(sstat_instance.percentile(s, 0.5)).to be 16.26
    expect(sstat_instance.percentile(s, 0.75)).to be 17.166
    expect(sstat_instance.percentile(s, 1)).to be 18.023
  end

  it 'check cdf for uniformly normal distribution Q function' do
    sstat_instance = SStat::Dist.new
    res = sstat_instance.cdf_unormal_Q(3.860003615)
    expect(res.round(7)).to be 0.0000567
  end

  it 'check z log rank function' do
    sstat_instance = SStat::Surv.new
    testing_file = Dir.pwd + '/spec/testing_data/testing_log_rank.csv'
    time_1 = read_csv_by_column(testing_file, 'E_T_2')
    cens_1 = read_csv_by_column(testing_file, 'Cens_2')
    time_2 = read_csv_by_column(testing_file, 'E_T_1')
    cens_2 = read_csv_by_column(testing_file, 'Cens_1')

    time_1 = array_to_double(time_1)
    cens_1 = array_to_censored(cens_1)

    time_2 = array_to_double(time_2)
    cens_2 = array_to_censored(cens_2)

    res = sstat_instance.log_rank_test(time_1, cens_1,time_2,cens_2)
    expect(res.round(3)).to be -3.860
  end

  it 'check index_less_equal function' do
  	sstat_instance = SStat::Surv.new
  	testing_file = Dir.pwd + '/spec/testing_data/testing_dataset_1.csv'
  	array = read_csv_by_column(testing_file, 'Median')
  	array = array_to_double(array)
  	s = array.sort
    expect(sstat_instance.index_less_equal(s, 15.254)).to be 1101
    expect(sstat_instance.index_less_equal(s, 16.26)).to be 2199
    expect(sstat_instance.index_less_equal(s, 17.166)).to be 3300
    expect(sstat_instance.index_less_equal(s, 18.023)).to be 4395
  end

  it 'check histogram_mean function' do
    sstat_instance = SStat::Hist.new
    bin = [2, 3, 5, 2, 1]
    range = [0, 1, 2, 3, 4, 5]
    expect(sstat_instance.hist_median(bin, range).round(3)).to be 2.500

    expect(sstat_instance.hist_mean(bin, range).round(3)).to be 2.269
  end

  it 'check kaplan meier function' do
    sstat_instance = SStat::Surv.new
    testing_file = Dir.pwd + '/spec/testing_data/testing_dataset_1.csv'
    time = read_csv_by_column(testing_file, 'E_T')
    cens = read_csv_by_column(testing_file, 'Cens')
    time = array_to_double(time)
    cens = array_to_censored(cens)
    res = sstat_instance.kaplan_meier(time, cens)
    expect(res["time"].last.round(3)).to be 18.612
    expect(res["prob"].first.round(3)).to be 0.995

    time_2 = [5, 6, 8, 3, 22]
    cens_2 = [0, 1, 0, 0, 0]
    res = sstat_instance.kaplan_meier(time_2, cens_2)
    expect(res["time"].last.round(3)).to be 22.000
    expect(res["prob"][0].round(3)).to be 0.8
    expect(res["prob"][1].round(3)).to be 0.6
  end

  it 'check kaplan meier function 3p extrapolation' do
    sstat_instance = SStat::Surv.new
    testing_file = Dir.pwd + '/spec/testing_data/testing_dataset_1.csv'
    time = read_csv_by_column(testing_file, 'E_T')
    cens = read_csv_by_column(testing_file, 'Cens')
    time = array_to_double(time)
    cens = array_to_censored(cens)
    res = sstat_instance.kaplan_meier_3p_extraploation(time, cens)
    expect(res["time"].last.round(3)).to be 18.765
    expect(res["prob"].last.round(3)).to be 0.000

    testing_file_2 = Dir.pwd + '/spec/testing_data/All_Data_updated_may2011_simple.csv'
    time_2 = read_csv_by_column(testing_file_2, 'SURVIVAL')
    cens_2 = read_csv_by_column(testing_file_2, 'CENSORED')
    time_2 = array_to_double(time_2)
    cens_2 = array_to_censored(cens_2)
    res_2 = sstat_instance.kaplan_meier_3p_extraploation(time_2, cens_2)
    expect(res_2["prob"].last.round(3)).to be 0.000

    testing_file_3 = Dir.pwd + '/spec/testing_data/subset_of_G1_Tx1_OS_U18r_PostMELD.csv'
    time_3 = read_csv_by_column(testing_file_3, 'Time')
    cens_3 = read_csv_by_column(testing_file_3, 'Censored')
    time_3 = array_to_double(time_3)
    cens_3 = array_to_censored(cens_3)
    res_3 = sstat_instance.kaplan_meier_3p_extraploation(time_3, cens_3)
  end
end