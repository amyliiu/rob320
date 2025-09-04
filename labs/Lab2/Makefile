CXX = g++
CXXFLAGS = -std=c++11
PROGRAMS = valid_parentheses moving_average group_anagrams two_sum relative_ranks

all: $(PROGRAMS)

valid_parentheses: valid_parentheses.cpp
	$(CXX) $(CXXFLAGS) -o valid_parentheses valid_parentheses.cpp

moving_average: moving_average.cpp
	$(CXX) $(CXXFLAGS) -o moving_average moving_average.cpp

group_anagrams: group_anagrams.cpp
	$(CXX) $(CXXFLAGS) -o group_anagrams group_anagrams.cpp

two_sum: two_sum.cpp
	$(CXX) $(CXXFLAGS) -o two_sum two_sum.cpp

relative_ranks: relative_ranks.cpp
	$(CXX) $(CXXFLAGS) -o relative_ranks relative_ranks.cpp

clean:
	rm -f $(PROGRAMS)

.PHONY: all clean