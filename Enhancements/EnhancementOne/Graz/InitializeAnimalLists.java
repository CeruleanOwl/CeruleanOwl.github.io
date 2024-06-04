/*
 * Author: Wesley Roberts
 * Date: 5/19/2024
*/
package Graz;
import java.util.ArrayList;

public class InitializeAnimalLists {
    // Adds dogs to a list for testing
    public static void initializeDogList(ArrayList<Dog> dogList) {
        Dog dog1 = new Dog("Spot", "German Shepherd", "male", "1", "25.6", "05-12-2019", "United States", "intake", false, "United States");
        Dog dog2 = new Dog("Rex", "Great Dane", "male", "3", "35.2", "02-03-2020", "United States", "Phase I", true, "United States");
        Dog dog3 = new Dog("Bella", "Chihuahua", "female", "4", "25.6", "12-12-2019", "Canada", "in service", false, "Canada");

        dogList.add(dog1);
        dogList.add(dog2);
        dogList.add(dog3);
    }

    // Adds monkeys to a list for testing
    public static void initializeMonkeyList(ArrayList<Monkey> monkeyList) {
        Monkey monkey1 = new Monkey("boots", "Guenon", "male", "5", "25.6", "05-12-2019", "United States", "intake", false, "United States", "45\"", "25\"", "20\"");
        Monkey monkey2 = new Monkey("Bobby Brown", "Tamarin", "male", "3", "35.2", "02-03-2020", "United States", "Phase I", true, "United States", "45\"", "25\"", "20\"");
        Monkey monkey3 = new Monkey("Kolton", "Squirrel monkey", "female", "4", "25.6", "12-12-2019", "Canada", "in service", false, "Canada", "45\"", "25\"", "20\"");

        monkeyList.add(monkey1);
        monkeyList.add(monkey2);
        monkeyList.add(monkey3);
    }

    // Adds cats to a list for testing
    public static void initializeCatList(ArrayList<Cat> catList) {
        Cat cat1 = new Cat("Jack", "Hairless Cat", "male", "27", "100.3", "06-15-2015", "Netherlands", "in service", true, "Egypt");
        Cat cat2 = new Cat("Cucumber", "Grey-Speckled Cat", "female", "1", "3", "05-19-2024", "United States", "in service", false, "United States");
        Cat cat3 = new Cat("Jarod", "Mancoon", "Male", "26", "60.4", "12-25-2024", "India", "in service", false, "India");

        catList.add(cat1);
        catList.add(cat2);
        catList.add(cat3);
    }
}
