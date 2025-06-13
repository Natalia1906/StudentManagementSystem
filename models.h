/**
 * @file models.h
 * @brief Заголовочный файл с определениями классов для Student Management System.
 */

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <limits>


/**
 * @brief Пауза до нажатия Enter.
 */
inline void pause() {
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * @class User
 * @brief Базовый класс, хранящий логин/пароль и виртуальное меню.
 */
class User {
public:
    /**
     * @enum Role
     * @brief Перечисление ролей пользователя.
     */
    enum class Role { Student, Teacher, Parent };

     /**
     * @brief Конструктор пользователя.
     * @param l Логин пользователя.
     * @param p Пароль пользователя.
     * @param r Роль пользователя.
     */
    User(std::string l, std::string p, Role r)
        : login_(std::move(l)), pass_(std::move(p)), role_(r) {}
    
    /**
     * @brief Виртуальный деструктор.
     */
    virtual ~User() = default;

    /**
     * @brief Проверяет совпадение логина и пароля.
     * @param l Введённый логин.
     * @param p Введённый пароль.
     * @return true, если логин и пароль совпадают, иначе false.
     */
    bool auth(const std::string& l, const std::string& p) const {
        return l == login_ && p == pass_;
    }

    /**
     * @brief Получить логин пользователя.
     * @return Ссылка на логин пользователя.
     */
    const std::string& login() const { return login_; }
    Role role() const               { return role_; }

    /**
     * @brief Виртуальное меню пользователя. Реализуется наследниками.
     */
    virtual void menu() = 0;

protected:
    std::string login_, pass_;
    Role        role_;
};

/**
 * @class Student
 * @brief Хранит вектор оценок и статистические методы.
 */
class Student final : public User {
public:
    /**
     * @brief Конструктор студента.
     * @param l Логин студента.
     * @param p Пароль студента.
     * @param g Вектор оценок (по умолчанию пустой).
     */
    Student(std::string l, std::string p, std::vector<int> g = {})
        : User(std::move(l), std::move(p), Role::Student),
          grades_(std::move(g)) {}

    /**
     * @brief Добавить оценку студенту.
     * @param v Оценка (целое число от 1 до 5).
     */
    void add(int v) { if (v>=1 && v<=5) grades_.push_back(v); }

    /**
     * @brief Вычислить средний балл.
     * @return Средний балл (0.0 если нет оценок).
     */
    double average() const {
        return grades_.empty() ? 0.0
               : static_cast<double>(std::accumulate(grades_.begin(), grades_.end(), 0))
                 / grades_.size();
    }

    /**
     * @brief Найти минимальную оценку.
     * @return Минимальная оценка или std::nullopt, если нет оценок.
     */
    std::optional<int> min() const {
        if (grades_.empty()) return std::nullopt;
        return *std::min_element(grades_.begin(), grades_.end());
    }

    /**
     * @brief Найти минимальную оценку.
     * @return Минимальная оценка или std::nullopt, если нет оценок.
     */
    std::optional<int> max() const {
        if (grades_.empty()) return std::nullopt;
        return *std::max_element(grades_.begin(), grades_.end());
    }

    /**
     * @brief Получить все оценки.
     * @return Константная ссылка на вектор оценок.
     */
    const std::vector<int>& grades() const { return grades_; }

    /**
     * @brief Меню для студента.
     */
    void menu() override {
        while (true) {
            std::cout << "\n--- Student (" << login() << ") ---\n"
                      << "1) Grades  2) Stats  0) Logout\n> ";
            int c; std::cin >> c;
            if (c==0) break;
            if (c==1) {
                std::cout << "Grades: ";
                for(int g:grades_) std::cout << g << ' ';
                std::cout << '\n'; pause();
            } else if (c==2) {
                std::cout << "Avg:" << average()
                          << " Min:" << (min()?std::to_string(*min()):"-")
                          << " Max:" << (max()?std::to_string(*max()):"-")
                          << '\n'; pause();
            }
        }
    }

private:
    std::vector<int> grades_;
};

/**
 * @class Teacher
 * @brief Видит статистику всех студентов.
 */
class Teacher final : public User {
public:
    /**
     * @brief Конструктор учителя.
     * @param l Логин учителя.
     * @param p Пароль учителя.
     * @param list Вектор умных указателей на студентов.
     */
    Teacher(std::string l, std::string p, std::vector<std::shared_ptr<Student>> list)
        : User(std::move(l), std::move(p), Role::Teacher),
          students_(std::move(list)) {}

    /**
     * @brief Меню для учителя.
     */
    void menu() override {
        while (true) {
            std::cout << "\n--- Teacher (" << login() << ") ---\n"
                      << "1) Per-student  2) Class avg  0) Logout\n> ";
            int c; std::cin >> c;
            if (c==0) break;
            if (c==1) {
                int idx=1; for(auto&s:students_) std::cout<<idx++<<") "<<s->login()<<'\n';
                std::cout<<"Pick:"; std::cin>>idx;
                if(idx>=1 && idx<=static_cast<int>(students_.size())){
                    auto&s=students_[idx-1];
                    std::cout<<"Avg:"<<s->average()
                             <<" Min:"<<(s->min()?std::to_string(*s->min()):"-")
                             <<" Max:"<<(s->max()?std::to_string(*s->max()):"-")<<'\n';
                }
                pause();
            } else if (c==2) {
                double sum=0; for(auto&s:students_)sum+=s->average();
                std::cout<<"Class average:"
                         <<(students_.empty()?0.0:sum/students_.size())<<'\n';
                pause();
            }
        }
    }
private:
    std::vector<std::shared_ptr<Student>> students_;
};

/**
 * @class Parent
 * @brief Доступен только просмотр оценок/статистики ребёнка.
 */
class Parent final : public User {
public:
    /**
     * @brief Конструктор родителя.
     * @param l Логин родителя.
     * @param p Пароль родителя.
     * @param kid Указатель на студента (ребёнка).
     */
    Parent(std::string l, std::string p, std::shared_ptr<Student> kid)
        : User(std::move(l), std::move(p), Role::Parent), kid_(std::move(kid)) {}

    /**
     * @brief Меню для родителя.
     */
    void menu() override {
        while (true) {
            std::cout << "\n--- Parent (" << login() << ") ---\n"
                      << "1) Child grades  2) Child stats  0) Logout\n> ";
            int c; std::cin >> c;
            if(c==0) break;
            if(c==1){
                std::cout<<"Grades: "; for(int g:kid_->grades())std::cout<<g<<' ';
                std::cout<<'\n'; pause();
            }else if(c==2){
                std::cout<<"Avg:"<<kid_->average()
                         <<" Min:"<<(kid_->min()?std::to_string(*kid_->min()):"-")
                         <<" Max:"<<(kid_->max()?std::to_string(*kid_->max()):"-")<<'\n';
                pause();
            }
        }
    }
private:
    std::shared_ptr<Student> kid_;
};

/**
 * @class System
 * @brief Простейшая «БД» + авторизация/меню.
 */
class System {
public:
    /**
     * @brief Добавить пользователя в систему.
     * @param u Умный указатель на пользователя.
     */
    void add(const std::shared_ptr<User>& u){ users_.push_back(u); }

    /**
     * @brief Запустить основной цикл работы системы.
     */
    void run() {
        while(true){
            std::string l,p;
            std::cout<<"\nLogin(exit=quit): "; std::cin>>l;
            if(l=="exit") break;
            std::cout<<"Password: "; std::cin>>p;
            bool ok=false;
            for(auto&u:users_) if(u->auth(l,p)){ ok=true; u->menu(); }
            if(!ok) std::cout<<"Wrong credentials!\n";
        }
    }
private:
    std::vector<std::shared_ptr<User>> users_;
};