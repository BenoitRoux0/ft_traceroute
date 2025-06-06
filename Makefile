NAME = ft_traceroute

BASE_SRCS =	src/main.c \
			src/traceroute/loop.c \
			src/traceroute/send_probe.c \
			src/traceroute/clear_trc.c \
			src/utils.c \
			src/parse_host.c \

SRCS =		$(BASE_SRCS) \
			src/traceroute/init_trc.c \

SRCS_BONUS =	$(BASE_SRCS) \
				src/traceroute/init_trc_bonus.c \
				src/traceroute/parse_opt.c \

INCS = inc/ft_traceroute.h

OBJS_DIR = .objs
OBJS_DIR_BONUS = .objs_bonus

OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)
OBJS_BONUS = $(SRCS_BONUS:%.c=$(OBJS_DIR_BONUS)/%.o)

CFLAGS = -Wall -Wextra -Werror -g3

REF = inetutils-2.0

mandatory:	$(NAME)

all:		$(NAME) $(NAME)_bonus

bonus:		$(NAME)_bonus

.objs/%.o:	%.c $(INCS)
			@mkdir -p $(basename $@)
			$(CC) $(CFLAGS) -o $@ -c $< -Iinc

.objs_bonus/%.o:	%.c $(INCS)
			@mkdir -p $(basename $@)
			$(CC) $(CFLAGS) -DBONUS=1 -o $@ -c $< -Iinc

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) -o $@ $(OBJS)

$(NAME)_bonus:	$(OBJS_BONUS)
				$(CC) $(CFLAGS) -o $@ $(OBJS_BONUS)

clean:
			$(RM) -r $(OBJS_DIR)
			$(RM) -r $(OBJS_DIR_BONUS)

fclean:		clean
			$(RM) $(NAME)
			$(RM) $(NAME)_bonus

re:			fclean
			$(MAKE) $(NAME)

re_bonus:	fclean
			$(MAKE) $(NAME)_bonus

inetutils-%:
			wget https://ftp.gnu.org/gnu/inetutils/$@.tar.gz
			tar -xf $@.tar.gz
			$(RM) $@.tar.gz

$(REF)/Makefile: $(REF)
			cd $(REF) && ./configure

$(REF)/src/traceroute: $(REF)/Makefile
			cd $(REF) && make -j

true_traceroute: $(REF)/src/traceroute
			ln -s $(REF)/src/traceroute true_traceroute

monitor:
			sudo tcpdump -nni any -v udp

.PHONY:	all clean fclean re build-ref
