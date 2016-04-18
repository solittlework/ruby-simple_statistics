#!/usr/bin/env ruby
loop_num = 10000
count = 0

for i in 1..loop_num
	for j in 1..loop_num
		count = count +1
	end
end

puts count