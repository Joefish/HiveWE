#pragma once

class BinaryReader {
public:
	std::vector<uint8_t> buffer;
	long long int position = 0;

	explicit BinaryReader(const std::vector<uint8_t>& buffer) : buffer(buffer) {}

	template<typename T>
	T read() {
		if (position + sizeof(T) > buffer.size()) {
			throw std::out_of_range("Trying to read out of range of buffer");
		}
		T result = *reinterpret_cast<T*>(&buffer[position]);
		position += sizeof(T);
		return result;
	}

	std::string read_string(const size_t size) {
		if (position + size > buffer.size()) {
			throw std::out_of_range("Trying to read out of range of buffer");
		}
		std::string result = { reinterpret_cast<char*>(&buffer[position]), static_cast<size_t>(size) };

		if (const size_t pos = result.find_first_of('\0', 0); pos != std::string::npos) {
			result.resize(pos);
		}

		position += size;
		return result;
	}

	std::string read_c_string() {
		std::string string(reinterpret_cast<char*>(buffer.data() + position));
		position += string.size();

		if (position > buffer.size()) {
			throw std::out_of_range("Trying to read out of range of buffer");
		}

		return string;
	}

	template<typename T>
	std::vector<T> read_vector(const size_t size) {
		if (position + sizeof(T) * size > buffer.size()) {
			throw std::out_of_range("Trying to read out of range of buffer");
		}
		std::vector<T> result(reinterpret_cast<T*>(&buffer[position]), reinterpret_cast<T*>(&buffer[position]) + size);
		position += sizeof(T) * size;
		return result;
	}

	long long remaining() const {
		return buffer.size() - position;
	}

	void advance(const size_t amount) {
		if (amount > 9999999) {
			throw std::invalid_argument("Amount is very large");
		}
		position += amount;
	}
};