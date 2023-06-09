include package.conf

NAME = LUAGRAN
OBJS = $(NAME).o lua/liblua.a
LUAHEADERS = lauxlib.h lua.h luaconf.h lualib.h
CMIXOBJS += $(PROFILE_O)
CXXFLAGS += -I. -Wall 
PROGS = $(NAME) lib$(NAME).so 

all: lib$(NAME).so

standalone: $(NAME)

lib$(NAME).so: $(OBJS) $(GENLIB)
	$(CXX) $(SHARED_LDFLAGS) -o $@ $(OBJS) $(GENLIB) $(SYSLIBS)


$(NAME): $(OBJS) $(CMIXOBJS)
	$(CXX) -o $@ $(OBJS) $(CMIXOBJS) $(LDFLAGS)



$(OBJS): $(INSTRUMENT_H) $(NAME).h 


clean:
	$(RM) $(OBJS) $(PROGS)

