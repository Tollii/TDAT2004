import javax.persistence.*;
import java.util.List;


public class AccountDao {
    private EntityManagerFactory emf;

    public AccountDao(EntityManagerFactory emf) {
        this.emf = emf;
    }

    public void createAccount(Account account) {
        EntityManager em = getEM();

        try{
            em.getTransaction().begin();
            em.persist(account);
            em.getTransaction().commit();
        } catch (Exception e){
            System.out.println(e);
        } finally {
            closeEM(em);
        }
    }

    public List<Account> getAccountByBalance(int balance) {
        EntityManager em = getEM();

        try {
            Query q = em.createQuery("SELECT OBJECT(o) FROM Account o where o.balance > :balance");
            q.setParameter("balance", balance);
            return q.getResultList();
        } finally {
            closeEM(em);
        }
    }

    public void changeAccount(Account account) {
        EntityManager em = getEM();

        try {
            em.getTransaction().begin();
            Account change = em.merge(account);
            em.getTransaction().commit();
        } finally {
            closeEM(em);
        }
    }


    private EntityManager getEM() {
        return emf.createEntityManager();
    }

    private void closeEM(EntityManager em) {
        if (em != null && em.isOpen()) em.close();
    }
}