import console_GUI

if __name__ == '__main__':

    while True:
        choice = console_GUI.welcome_menu()
        print("Main menu: ", choice)
        if choice == 5:
            break
        elif choice == 1:
            console_GUI.scrapping()
        elif choice == 2:
            console_GUI.preprocessing()
        elif choice == 3:
            console_GUI.search_engine_configuration()
        elif choice == 4:
            console_GUI.process_search()


