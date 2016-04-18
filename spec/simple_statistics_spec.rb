require 'spec_helper'
require 'sstat' #simple statistics
require 'csv' #used to read file

def read_csv_by_column(csv_file, col_num)
	csv = CSV.read('csv_file', :headers=>true)
	return csv[col_num]
end

describe SimpleStatistics do
  it 'has a version number' do
    expect(SimpleStatistics::VERSION).not_to be nil
  end

  it 'check function' do
    expect(Sstat.calculate_sui_tax(9000, 0.34)).to be 3060.0
  end

  it 'check percentile function' do
  	arr = [1.0,3.0,4.0,5.0,6.0];
  	puts Sstat.sum(arr);
    expect(Sstat.calculate_sui_tax(9000, 0.34)).to be 3060.0
  end
end
