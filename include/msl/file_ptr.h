#pragma once

#include <cstdio>
#include <vector>

namespace msl
{
class file_ptr
{
	std::FILE * m_ptr{nullptr};

public:
#ifdef _WIN32
	file_ptr(const char * filename, const char * mode = "r") { fopen_s(&m_ptr, filename, mode); }
#else
	file_ptr(const char * filename, const char * mode = "r") { m_ptr = std::fopen(filename, mode); }
#endif
	file_ptr(std::FILE * ptr) { m_ptr = ptr; }
	~file_ptr() { std::fclose(m_ptr); }
	std::FILE * operator*() { return m_ptr; }

	std::FILE * get() { return m_ptr; }
	void reset()
	{
		if (m_ptr)
		{
			std::fclose(m_ptr);
			m_ptr = nullptr;
		}
	}

	bool is_open() const { return m_ptr; }

	std::size_t size() const
	{
		std::size_t cur = std::ftell(m_ptr); // get cur pos
		std::fseek(m_ptr, 0, SEEK_END); // go to EOF
		std::size_t filesize = std::ftell(m_ptr); // get filesize
		std::fseek(m_ptr, cur, SEEK_SET); // go to old pos
		return filesize;
	}

	std::vector<char> read(std::size_t n = 0)
	{
		if (n == 0) // read whole remaining file
			n = this->size();
		std::vector<char> buf(n);
		fread(buf.data(), 1, buf.size(), m_ptr);
		return buf;
	}

	std::string string_read(std::size_t n = 0)
	{
		auto vec = this->read(n);
		vec.emplace_back('\0');
		return std::string(vec.begin(), vec.end());
	}
};
} // namespace msl
