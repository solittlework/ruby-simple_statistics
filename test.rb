#!/usr/bin/env ruby

loop_num = 10000
arr = []
c=0
for i in 0..loop_num
	arr.push(i)
end

for i in 0..loop_num
	for j in 0..loop_num
		c+=arr[i]
	end
end

puts c