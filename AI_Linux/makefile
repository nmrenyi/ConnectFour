MY_PLAYER = so/Strategy.so
PA = TestCases/2.so
PB = TestCases/4.so
ME = TestCases/us.so
ROUND = 10

.PHONY: compete strategy compete_debug strategy_debug clean
all: compete strategy

compete:
	$(MAKE) -C Compete

strategy:
	$(MAKE) -C Strategy so

debug: compete_debug strategy_debug

compete_debug:
	$(MAKE) -C Compete debug

strategy_debug:
	$(MAKE) -C Strategy debug

clean:
	$(MAKE) -C Strategy clean
	$(MAKE) -C Compete clean
play:
	./Compete/Compete $(MY_PLAYER) $(PB) result.txt $(ROUND)
play_my:
	./Compete/Compete $(MY_PLAYER) $(ME) result.txt $(ROUND)