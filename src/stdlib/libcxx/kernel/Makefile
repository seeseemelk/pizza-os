include $(DEFAULTMAKE)

SRC := $(wildcard *.cpp)
OBJ := $(SRC:%.cpp=$(BUILDDIR)/kernel/%.o)

$(LIBKCXX): $(OBJ)
	$(STATUS_AR)
	$(MKDIR)
	$(AR) rc $@ $^

$(BUILDDIR)/kernel/%.o: %.cpp
	$(STATUS_KCXX)
	$(MKDIR)
	$(CXX) $(CXXFLAGS_KERNEL) -MMD -c -o $@ $<

-include $(OBJ:.o=.d)
