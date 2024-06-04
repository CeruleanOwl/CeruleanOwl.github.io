/*
 * Author: Wesley Roberts
 * Date: 5/19/2024
*/
package Graz;
import java.util.ArrayList;
import java.util.Scanner;

public class Driver {
    //Initialize lists for Animals
    private static ArrayList<Dog> dogList = new ArrayList<Dog>();
    private static ArrayList<Monkey> monkeyList = new ArrayList<Monkey>();
	private static ArrayList<Cat> catList = new ArrayList<Cat>();

    public static void main(String[] args) {
    	String userInput = "";

		//Initializations for driver class
    	Scanner scnr = new Scanner(System.in);
        InitializeAnimalLists.initializeDogList(dogList);
        InitializeAnimalLists.initializeMonkeyList(monkeyList);
		InitializeAnimalLists.initializeCatList(catList);

		//Displays welcome message
		Display.welcomeMessage();

        // Accepts user input with given parameters
        while (!userInput.equalsIgnoreCase("q")) {
        	Display.displayMenu();
        	userInput = scnr.nextLine();
        	
        	switch (userInput) {
        	case "1":
        		IntakeNewAnimals.intakeNewDog(scnr, dogList);
        		break;
        	case "2":
        		IntakeNewAnimals.intakeNewMonkey(scnr, monkeyList);
        		break;
			case "3":
        		IntakeNewAnimals.intakeNewCat(scnr, catList);
        		break;
        	case "4":
        		ReserveAnimals.reserveAnimal(scnr, dogList, monkeyList, catList);
        		break;
        	case "5":
        		Display.printAnimals("Dogs", dogList, monkeyList, catList);
        		break;
        	case "6":
        		Display.printAnimals("Monkeys", dogList, monkeyList, catList);
        		break;
			case "7":
        		Display.printAnimals("Cats", dogList, monkeyList, catList);
        		break;
        	case "8":
        		Display.printAnimals("Not Reserved", dogList, monkeyList, catList);
        		break;
        	case "q":
        		break;
        	default:
        		System.out.println("Please enter a valid input.");	
        	}

        }

    }

}

