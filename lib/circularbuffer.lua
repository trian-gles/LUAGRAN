local CircularBuffer = {}

function CircularBuffer.create(n)
	local buf = {}
	setmetatable(buf, CircularBuffer)
	buf.index = 0
	buf.size = 0
	buf.maxsize = n
	return buf
end

function CircularBuffer.add(self, v)
	self[self.index] = v
	if self.size < self.maxsize then
		self.size += 1
	end


	self.index += 1
	self.index = self.index % self.maxsize
end
