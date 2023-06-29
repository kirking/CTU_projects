TARGET = ./poligkir
SRC = src
OBJDIR = obj
DOC = doc
CXX=g++ -std=c++14
LD=g++
CXXFLAGS=-Wall -pedantic -g
CC_SDL=`sdl2-config --cflags` -lSDL2 -lSDL2_image
OBJECTS =\
                        CBonus.o\
                        CEnergizer.o\
                        CMaze.o\
                        CPinky.o\
                        CInky.o\
                        CLogic.o\
                        main.o
all: compile doc

#Clean all objects and documentation
clean:
	rm -r $(OBJECTS) rm -rf $(TARGET)
	rm -rf $(DOC)
	rm -rf *.dSYM


compile: $(TARGET)

run: $(TARGET)
	$(TARGET)

#Create documentation

doc: $(SRC)/Logic/Bonuses/CBonus.cpp $(SRC)/Logic/Bonuses/CBonus.h $(SRC)/Logic/Bonuses/CEnergizer.h \
$(SRC)/Logic/Bonuses/CEnergizer.cpp $(SRC)/Logic/Bonuses/CFruit.h \
$(SRC)/Logic/Map/CGate.h $(SRC)/Logic/Map/CMaze.cpp $(SRC)/Logic/Map/CMaze.h $(SRC)/Logic/Map/CRoad.h \
$(SRC)/Logic/Map/CTile.h $(SRC)/Logic/Map/CTunnel.h $(SRC)/Logic/Map/CWall.h \
$(SRC)/Logic/Units/CBlinky.h $(SRC)/Logic/Units/CPinky.h $(SRC)/Logic/Units/CPinky.cpp \
$(SRC)/Logic/Units/CClyde.h $(SRC)/Logic/Units/CInky.h $(SRC)/Logic/Units/CInky.cpp \
$(SRC)/Logic/Units/CPacman.h $(SRC)/Logic/Units/CUnit.h $(SRC)/Logic/Units/EGhostMode.h \
$(SRC)/Logic/Units/CGhost.h $(SRC)/CCoords.h $(SRC)/CGame.h $(SRC)/CGameDifficulty.h \
$(SRC)/CGraphics.h $(SRC)/CLogic.cpp $(SRC)/CLogic.h $(SRC)/CMenu.h $(SRC)/CMenuGraphics.h \
$(SRC)/CTimer.h $(SRC)/EDirection.h $(SRC)/main.cpp

	rm -Rf $(DOC)
	mkdir $(DOC)
	doxygen Doxyfile
	mv html/* $(DOC)
	rm -r html/

#Objects's compilation rules

$(TARGET) : $(OBJECTS)
	$(LD) -o $(TARGET) $(OBJECTS) $(CC_SDL)

CBonus.o : $(SRC)/Logic/Bonuses/CBonus.cpp \
				     $(SRC)/Logic/Bonuses/CBonus.h $(SRC)/Logic/Map/CMaze.h

	$(CXX) $(CXXFLAGS) -c -o $@ $<

CEnergizer.o : $(SRC)/Logic/Bonuses/CEnergizer.cpp \
				     $(SRC)/Logic/Bonuses/CEnergizer.h $(SRC)/Logic/Map/CMaze.h $(SRC)/Logic/Bonuses/CBonus.h

	$(CXX) $(CXXFLAGS) -c -o $@ $<



	$(CXX) $(CXXFLAGS) -c -o $@ $<

CMaze.o : $(SRC)/Logic/Map/CMaze.cpp \
				     $(SRC)/Logic/Map/CMaze.h  $(SRC)/Logic/Bonuses/CEnergizer.h \
				     $(SRC)/Logic/Bonuses/CFruit.h $(SRC)/Logic/Units/EGhostMode.h \
				     $(SRC)/Logic/Map/CGate.h $(SRC)/Logic/Map/CRoad.h \
				     $(SRC)/Logic/Map/CTunnel.h $(SRC)/Logic/Map/CWall.h $(SRC)/CLogic.h

	$(CXX) $(CXXFLAGS) $(CC_SDL) -c -o $@ $<

CPinky.o : $(SRC)/Logic/Units/CPinky.cpp \
				     $(SRC)/Logic/Units/CPinky.h $(SRC)/Logic/Units/CGhost.h

	$(CXX) $(CXXFLAGS) $(CC_SDL) -c -o $@ $<

CInky.o : $(SRC)/Logic/Units/CInky.cpp \
				     $(SRC)/Logic/Units/CInky.h $(SRC)/Logic/Units/CGhost.h

	$(CXX) $(CXXFLAGS) $(CC_SDL) -c -o $@ $<

CLogic.o : $(SRC)/CLogic.cpp \
				    $(SRC)/CLogic.h  $(SRC)/Logic/Units/CPacman.h \
				    $(SRC)/Logic/Units/CBlinky.h $(SRC)/Logic/Units/CPinky.h \
				    $(SRC)/Logic/Units/CInky.h $(SRC)/Logic/Units/CClyde.h \
				    $(SRC)/CGameDifficulty.h $(SRC)/Logic/Map/CMaze.h

	$(CXX) $(CXXFLAGS) $(CC_SDL) -c -o $@ $<

main.o : $(SRC)/main.cpp $(SRC)/CGame.h

	$(CXX) $(CXXFLAGS) $(CC_SDL) -c -o $@ $<
