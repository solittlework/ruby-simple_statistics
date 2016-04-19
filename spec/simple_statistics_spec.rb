require 'spec_helper'
require 'sstat' #simple statistics
require 'csv' #used to read file

def read_csv_by_column(csv_file, col_num)
	csv = CSV.read(csv_file, :headers=>true)
	return csv[col_num]
end

def array_to_double(array)
	float_array = []
	array.map{ |x| float_array.push(x.to_f)}
	return float_array
end

describe SimpleStatistics do
  it 'has a version number' do
    expect(SimpleStatistics::VERSION).not_to be nil
  end

  it 'check percentile function' do
  	sstat_instance = SStat::Basic.new
  	testing_file = Dir.pwd + '/spec/testing_data/testing_dataset_1.csv'
  	array = read_csv_by_column(testing_file, 'Median')
  	array = array_to_double(array)
  	s = array.sort

    expect(sstat_instance.percentile(s, 0.25)).to be 15.254
    expect(sstat_instance.percentile(s, 0.5)).to be 16.26
    expect(sstat_instance.percentile(s, 0.75)).to be 17.166
    expect(sstat_instance.percentile(s, 1)).to be 18.023
  end

  it 'check index_less_equal function' do
  	sstat_instance = SStat::Basic.new
  	testing_file = Dir.pwd + '/spec/testing_data/testing_dataset_1.csv'
  	array = read_csv_by_column(testing_file, 'Median')
  	array = array_to_double(array)
  	s = array.sort
    expect(sstat_instance.index_less_equal(s, 15.254)).to be 1101
    expect(sstat_instance.index_less_equal(s, 16.26)).to be 2199
    expect(sstat_instance.index_less_equal(s, 17.166)).to be 3300
    expect(sstat_instance.index_less_equal(s, 18.023)).to be 4395
  end



end
