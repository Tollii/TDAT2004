
import javax.persistence.*;
import java.io.*;



@Entity
public class Account implements Serializable {
    @TableGenerator(
            name = "yourTableGenerator",
            allocationSize = 1,
            initialValue = 1)
    @Id
    @GeneratedValue(
            strategy=GenerationType.TABLE,
            generator="yourTableGenerator")
    private int id;
    @Version // Task 4
    private int lockField; // Task 4
    private String name;
    private String accountNumber;
    private double balance;


    public Account(){

    }

    public Account(String name, String accountNumber, double balance){
        this.name = name;
        this.accountNumber = accountNumber;
        this.balance = balance;
    }


    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getAccountNumber() {
        return accountNumber;
    }

    public void setAccountNumber(String accountNumber) {
        this.accountNumber = accountNumber;
    }

    public double getBalance() {
        return balance;
    }

    public void setBalance(double balance) {
        this.balance = balance;
    }

    public void deduct(double amount){
        this.balance -= amount;
    }

    public void add(double amount){
        this.balance += amount;
    }

    public String toString(){
        return "name: " +name + " accountNumber: " + accountNumber + " Balance: " + balance;
    }



}