#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<map>
#include<assert.h>

std::vector<std::string>readFileLines(const std::string& path) {
	std::vector<std::string> lines;

	std::fstream fileHandler(path.c_str());

	if (fileHandler.fail()) {
		std::cout << "\n\nERROR: Can't open the file\n\n";
		return lines;
	}
	std::string line;

	while (std::getline(fileHandler, line)) {
		if (line.size() == 0)
			continue;
		lines.push_back(line);
	}

	fileHandler.close();
	return lines;
}

void writeFileLines(const std::string& path, const std::vector<std::string>& lines, bool append = true) {
	auto status = std::ios::in | std::ios::out | std::ios::app;

	if (!append)
		status = std::ios::in | std::ios::out | std::ios::trunc;	// overwrite

	std::fstream fileHandler(path.c_str(), status);

	if (fileHandler.fail()) {
		std::cout << "\n\nERROR: Can't open the file\n\n";
		return;
	}
	for (const auto& line : lines)
		fileHandler << line << "\n";

	fileHandler.close();
}

std::vector<std::string>splitString(const std::string&str, const std::string& delimiter = ",") {
	std::string s = str;
	std::vector<std::string>strs;

	int pos = 0;
	std::string substr;
	while ((pos = (int)s.find(delimiter)) != -1) {
		substr = s.substr(0, pos);
		strs.push_back(substr);
		s.erase(0, pos + delimiter.length());
	}
	strs.push_back(s);
	return strs;
}

int toInt(const std::string& str) {
	std::istringstream iss(str);
	int num;
	iss >> num;

	return num;
}

int readInt(int low, int high) {
	std::cout << "\nEnter number in range " << low << " - " << high << ": ";
	int value;

	std::cin >> value;

	if (low <= value && value <= high)
		return value;

	std::cout << "ERROR: invalid number...Try again\n";
	return readInt(low, high);
}

int showReadMenu(const std::vector<std::string>& choices) {
	std::cout << "\nMenu:\n";
	for (int ch = 0; ch < (int)choices.size(); ++ch) {
		std::cout << "\t" << ch + 1 << ": " << choices[ch] << "\n";
	}
	return readInt(1, choices.size());
}
//////////////////////////////////////////////////////////////

class Question {
private:
	int question_id;
	// To support thread. Each question look to a parent question
	// -1 No parent (first question in the thread)
	int parent_question_id;
	int from_user_id;
	int to_user_id;
	int is_anonymous_questions;	// 0 or 1
	std::string question_text;
	std::string answer_text;			// empty = not answered

public:
	/*
	 * How many conclassor arguments is too many?
	 * 	https://stackoverflow.com/questions/40264/how-many-conclassor-arguments-is-too-many
	 */
	Question( ) :
		question_id(-1), parent_question_id(-1), from_user_id(-1), to_user_id(-1), is_anonymous_questions(1) {
	}

	Question(const std::string& line) {
		std::vector<std::string> substrs = splitString(line);
		assert(substrs.size() == 7);

		question_id = toInt(substrs[0]);
		parent_question_id = toInt(substrs[1]);
		from_user_id = toInt(substrs[2]);
		to_user_id = toInt(substrs[3]);
		is_anonymous_questions = toInt(substrs[4]);
		question_text = substrs[5];
		answer_text = substrs[6];
	}

	// It is more proper (or safer) to use the getters/setters in the class itself. For simplicity, I did not apply this in the code.

	std::string ToString() const {
		std::ostringstream oss;
		oss << question_id << "," << parent_question_id << "," << from_user_id << "," << to_user_id << "," << is_anonymous_questions << "," << question_text << "," << answer_text;

		return oss.str();
	}

	void PrintToQuestion() const {
		std::string prefix = "";

		if (parent_question_id != -1)
			prefix = "\tThread: ";

		std::cout << prefix << "Question Id (" << question_id << ")";
		if (!is_anonymous_questions)
			std::cout << " from user id(" << from_user_id << ")";
		std::cout << "\t Question: " << question_text << "\n";

		if (answer_text != "")
			std::cout << prefix << "\tAnswer: " << answer_text << "\n";
		std::cout << "\n";
	}

	void PrintFromQuestion() const {
		std::cout << "Question Id (" << question_id << ")";
		if (!is_anonymous_questions)
			std::cout << " !AQ";

		std::cout << " to user id(" << to_user_id << ")";
		std::cout << "\t Question: " << question_text;

		if (answer_text != "")
			std::cout << "\tAnswer: " << answer_text << "\n";
		else
			std::cout << "\tNOT Answered YET\n";
	}

	void PrintFeedQuestion() const {
		if (parent_question_id != -1)
			std::cout << "Thread Parent Question ID (" << parent_question_id << ") ";

		std::cout << "Question Id (" << question_id << ")";
		if (!is_anonymous_questions)
			std::cout << " from user id(" << from_user_id << ")";

		std::cout << " To user id(" << to_user_id << ")";

		std::cout << "\t Question: " << question_text << "\n";
		if (answer_text != "")
			std::cout << "\tAnswer: " << answer_text << "\n";
	}
	/////////////////
	// Setters & Getters

	const std::string& GetAnswerText() const {
		return answer_text;
	}

	void SetAnswerText(const std::string& answerText) {
		answer_text = answerText;
	}

	int GetFromUserId() const {
		return from_user_id;
	}

	void SetFromUserId(int fromUserId) {
		from_user_id = fromUserId;
	}

	int GetIsAnonymousQuestions() const {
		return is_anonymous_questions;
	}

	void SetIsAnonymousQuestions(int isAnonymousQuestions) {
		is_anonymous_questions = isAnonymousQuestions;
	}

	int GetParentQuestionId() const {
		return parent_question_id;
	}

	void SetParentQuestionId(int parentQuestionId) {
		parent_question_id = parentQuestionId;
	}

	int GetQuestionId() const {
		return question_id;
	}

	void SetQuestionId(int questionId) {
		question_id = questionId;
	}

	const std::string& GetQuestionText() const {
		return question_text;
	}

	void SetQuestionText(const std::string& questionText) {
		question_text = questionText;
	}

	int GetToUserId() const {
		return to_user_id;
	}

	void SetToUserId(int toUserId) {
		to_user_id = toUserId;
	}
};

class User {
private:
	int user_id;		// internal system ID
	std::string user_name;
	std::string password;
	std::string name;
	std::string email;
	int allow_anonymous_questions;	// 0 or 1

	std::vector<int> questions_id_from_me;
	// From question id to list of questions IDS on this question (thread questions) - For this user
	std::map<int, std::vector<int>> questionid_questionidsThead_to_map;

public:
	User() :
		user_id(-1), allow_anonymous_questions(-1) {
	}

	User(const std::string& line) {
		std::vector<std::string> substrs = splitString(line);
		assert(substrs.size() == 6);

		user_id = toInt(substrs[0]);
		user_name = substrs[1];
		password = substrs[2];
		name = substrs[3];
		email = substrs[4];
		allow_anonymous_questions = toInt(substrs[5]);
	}

	std::string ToString() const {
		std::ostringstream oss;
		oss << user_id << "," << user_name << "," << password << "," << name << "," << email << "," << allow_anonymous_questions;

		return oss.str();
	}

	void Print() const {
		std::cout << "User " << user_id << ", " << user_name << " " << password << ", " << name << ", " << email << "\n";
	}

	void ResetToQuestions(const std::vector<int>& to_questions) {
		questions_id_from_me.clear();

		for (const auto& question_id : to_questions)
			questions_id_from_me.push_back(question_id);
	}
	void ResetFromQuestions(const std::vector<std::pair<int, int>>& to_questions) {
		questionid_questionidsThead_to_map.clear();

		for (const auto& id_pair : to_questions)
			questionid_questionidsThead_to_map[id_pair.first].push_back(id_pair.second);
	}

	void ReadUser(const std::string& user_name, int id) {
		SetUserName(user_name);
		SetUserId(id);

		std::string str;

		std::cout << "Enter password: ";
		std::cin >> str;
		SetPassword(str);

		std::cout << "Enter name: ";
		std::cin >> str;
		SetName(str);

		std::cout << "Enter email: ";
		std::cin >> str;
		SetEmail(str);

		std::cout << "Allow anonymous questions? (0 or 1): ";
		int st;
		std::cin >> st;
		SetAllowAnonymousQuestions(st);
	}

	int IsAllowAnonymousQuestions() const {
		return allow_anonymous_questions;
	}

	void SetAllowAnonymousQuestions(int allowAnonymousQuestions) {
		allow_anonymous_questions = allowAnonymousQuestions;
	}

	const std::string& GetEmail() const {
		return email;
	}

	void SetEmail(const std::string& email) {
		this->email = email;
	}

	const std::string& GetName() const {
		return name;
	}

	void SetName(const std::string& name) {
		this->name = name;
	}

	const std::string& GetPassword() const {
		return password;
	}

	void SetPassword(const std::string& password) {
		this->password = password;
	}

	int GetUserId() const {
		return user_id;
	}

	void SetUserId(int userId) {
		user_id = userId;
	}

	const std::string& GetUserName() const {
		return user_name;
	}

	void SetUserName(const std::string& userName) {
		user_name = userName;
	}

	const std::map<int, std::vector<int> >& GetQuestionidQuestionidsTheadToMap() const {
		return questionid_questionidsThead_to_map;
		// Note no setter is provided / returned as const &
	}

	const std::vector<int>& GetQuestionsIdFromMe() const {
		return questions_id_from_me;
		// Note no setter is provided
	}
};

class QuestionsManager {
private:
	// From question id to list of questions IDS on this question (thread questions) - All users
	std::map<int, std::vector<int>> questionid_questionidsThead_to_map;

	// Map the question id to question object. Let's keep one place ONLY with the object
	// When you study pointers, easier handling
	std::map<int, Question> questionid_questionobject_map;

	int last_id;

public:
	QuestionsManager() {
		last_id = 0;
	}

	void LoadDatabase() {
		last_id = 0;
		questionid_questionidsThead_to_map.clear();
		questionid_questionobject_map.clear();

		std::vector<std::string> lines = readFileLines("question.txt");
		for (const auto& line : lines) {
			Question question(line);
			last_id = std::max(last_id, question.GetQuestionId());

			questionid_questionobject_map[question.GetQuestionId()] = question;

			if (question.GetParentQuestionId() == -1)
				questionid_questionidsThead_to_map[question.GetQuestionId()].push_back(question.GetQuestionId());
			else
				questionid_questionidsThead_to_map[question.GetParentQuestionId()].push_back(question.GetQuestionId());
		}
	}

	std::vector<int> GetUserToQuestions(const User& user) const {
		std::vector<int> question_ids;

		for (const auto& pair : questionid_questionidsThead_to_map) {	// pair<int, vector<Question>>
			for (const auto& question_id : pair.second) {		//  vector<Question>

				// Get the question from the map. & means same in memory, DON'T COPY
				const Question& question = questionid_questionobject_map.find(question_id)->second;

				if (question.GetFromUserId() == user.GetUserId())
					question_ids.push_back(question.GetQuestionId());
			}
		}
		return question_ids;
	}

	std::vector<std::pair<int, int>> GetUserFromQuestions(const User& user) const {
		std::vector<std::pair<int, int>> question_ids;

		for (const auto& pair : questionid_questionidsThead_to_map) {	// pair<int, vector<Question>>
			for (const auto& question_id : pair.second) {		//  vector<Question>
				// Get the question from the map. & means same in memory, DON'T COPY
				const Question& question = questionid_questionobject_map.find(question_id)->second;

				if (question.GetToUserId() == user.GetUserId()) {
					if (question.GetParentQuestionId() == -1)
						question_ids.push_back(std::make_pair(question.GetQuestionId(), question.GetQuestionId()));
					else
						question_ids.push_back(std::make_pair(question.GetParentQuestionId(), question.GetQuestionId()));
				}
			}
		}
		return question_ids;
	}

	void PrintUserToQuestions(const User& user) const {
		std::cout << "\n";

		if (user.GetQuestionidQuestionidsTheadToMap().size() == 0)
			std::cout << "No Questions";

		for (const auto& pair : user.GetQuestionidQuestionidsTheadToMap()) {		// pair<int, vector<Question>>
			for (const auto& question_id : pair.second) {		//  vector<Question>

				// Get the question from the map. & means same in memory, DON'T COPY
				// Accessing questionid_questionobject_map[] change the map by adding if not exist. You can't use in const function
				// Here we just find, which return iterator (second is like a pointer to object)
				const Question& question = questionid_questionobject_map.find(question_id)->second;
				question.PrintToQuestion();
			}
		}
		std::cout << "\n";
	}

	void PrintUserFromQuestions(const User& user) const {
		std::cout << "\n";
		if (user.GetQuestionsIdFromMe().size() == 0)
			std::cout << "No Questions";

		for (const auto& question_id : user.GetQuestionsIdFromMe()) {		//  vector<Question>
			const Question& question = questionid_questionobject_map.find(question_id)->second;
			question.PrintFromQuestion();
		}
		std::cout << "\n";
	}

	// Used in Answering a question for YOU.
	// It can be any of your questions (thread or not)
	int ReadQuestionIdAny(const User& user) const {
		int question_id;
		std::cout << "Enter Question id or -1 to cancel: ";
		std::cin >> question_id;

		if (question_id == -1)
			return -1;

		if (!questionid_questionobject_map.count(question_id)) {
			std::cout << "\nERROR: No question with such ID. Try again\n\n";
			return ReadQuestionIdAny(user);
		}
		const Question& question = questionid_questionobject_map.find(question_id)->second;

		if (question.GetToUserId() != user.GetUserId()) {
			std::cout << "\nERROR: Invalid question ID. Try again\n\n";
			return ReadQuestionIdAny(user);
		}
		return question_id;
	}

	// Used to ask a question on a specific thread for whatever user
	int ReadQuestionIdThread(const User& user) const {
		int question_id;
		std::cout << "For thread question: Enter Question id or -1 for new question: ";
		std::cin >> question_id;

		if (question_id == -1)
			return -1;

		if (!questionid_questionidsThead_to_map.count(question_id)) {
			std::cout << "No thread question with such ID. Try again\n";
			return ReadQuestionIdThread(user);
		}
		return question_id;
	}

	void AnswerQuestion(const User& user) {
		int question_id = ReadQuestionIdAny(user);

		if (question_id == -1)
			return;

		Question& question = questionid_questionobject_map.find(question_id)->second;

		question.PrintToQuestion();

		if (question.GetAnswerText() != "")
			std::cout << "\nWarning: Already answered. Answer will be updated\n";

		std::cout << "Enter answer: ";	// if user entered comma, system fails :)

		std::string line;
		std::getline(std::cin, line);
		std::getline(std::cin, line);
		question.SetAnswerText(line);
	}

	void DeleteQuestion(const User& user) {
		int question_id = ReadQuestionIdAny(user);

		if (question_id == -1)
			return;

		std::vector<int> ids_to_remove;	// to remove from questionid_questionobject_map

		// Let's see if thread or not. If thread, remove all of it
		if (questionid_questionidsThead_to_map.count(question_id)) { // thread
			ids_to_remove = questionid_questionidsThead_to_map[question_id];
			questionid_questionidsThead_to_map.erase(question_id);
		}
		else {
			ids_to_remove.push_back(question_id);

			// let's find in which thread to remove. Consistency is important when have multi-view
			for (auto& pair : questionid_questionidsThead_to_map) {
				std::vector<int>& vec = pair.second;
				for (int pos = 0; pos < (int)vec.size(); ++pos) {
					if (question_id == vec[pos]) {
						vec.erase(vec.begin() + pos);
						break;
					}
				}
			}
		}
		for (const auto& id : ids_to_remove)
			questionid_questionobject_map.erase(id);
	}

	void AskQuestion(const User& user, const std::pair<int, int>& to_user_pair) {
		Question question;

		if (!to_user_pair.second) {
			std::cout << "Note: Anonymous questions are not allowed for this user\n";
			question.SetIsAnonymousQuestions(0);
		}
		else {
			std::cout << "Is anonymous questions?: (0 or 1): ";
			int st;
			std::cin >> st;
			question.SetIsAnonymousQuestions(st);
		}

		question.SetParentQuestionId(ReadQuestionIdThread(user));

		std::cout << "Enter question text: ";	// if user entered comma, system fails :)
		std::string line;
		std::getline(std::cin, line);
		std::getline(std::cin, line);
		question.SetQuestionText(line);

		question.SetFromUserId(user.GetUserId());
		question.SetToUserId(to_user_pair.first);

		// What happens in 2 parallel sessions who asked question?
		// They are given same id. This is wrong handling :)
		question.SetQuestionId(++last_id);

		questionid_questionobject_map[question.GetQuestionId()] = question;

		if (question.GetParentQuestionId() == -1)
			questionid_questionidsThead_to_map[question.GetQuestionId()].push_back(question.GetQuestionId());
		else
			questionid_questionidsThead_to_map[question.GetParentQuestionId()].push_back(question.GetQuestionId());
	}

	void ListFeed() const {
		for (const auto& pair : questionid_questionobject_map) {
			const Question& question = pair.second;

			if (question.GetAnswerText() == "")
				continue;
			question.PrintFeedQuestion();
		}
	}

	void UpdateDatabase() {
		std::vector<std::string> lines;

		for (const auto& pair : questionid_questionobject_map)
			lines.push_back(pair.second.ToString());

		writeFileLines("question.txt", lines, false);
	}
};

class UsersManager {
private:
	std::map<std::string, User> userame_userobject_map;
	User current_user;
	int last_id;


public:
	UsersManager() {
		last_id = 0;
	}

	void LoadDatabase() {
		last_id = 0;
		userame_userobject_map.clear();

		std::vector<std::string> lines = readFileLines("users.txt");
		for (const auto& line : lines) {
			User user(line);
			userame_userobject_map[user.GetUserName()] = user;
			last_id = std::max(last_id, user.GetUserId());
		}
	}

	void AccessSystem() {
		int choice = showReadMenu({ "Login", "Sign Up" });
		if (choice == 1)
			DoLogin();
		else
			DoSignUp();
	}

	void DoLogin() {
		LoadDatabase();	// in case user added from other parallel run

		while (true) {
			std::string name, pass;
			std::cout << "Enter user name & password: ";
			std::cin >> name >> pass;
			current_user.SetUserName(name);
			current_user.SetPassword(pass);

			if (!userame_userobject_map.count(current_user.GetUserName())) {
				std::cout << "\nInvalid user name or password. Try again\n\n";
				continue;
			}
			const User& user_exist = userame_userobject_map[current_user.GetUserName()];

			if (current_user.GetPassword() != user_exist.GetPassword()) {
				std::cout << "\nInvalid user name or password. Try again\n\n";
				continue;
			}
			current_user = user_exist;
			break;
		}
	}

	void DoSignUp() {
		std::string user_name;
		while (true) {
			std::cout << "Enter user name. (No spaces): ";
			std::cin >> user_name;

			if (userame_userobject_map.count(user_name))
				std::cout << "Already used. Try again\n";
			else
				break;
		}
		// Move logic to user class, which may keep extending data members in future

		current_user.ReadUser(user_name, ++last_id);
		userame_userobject_map[current_user.GetUserName()] = current_user;

		UpdateDatabase(current_user);
	}

	void ResetToQuestions(const std::vector<int>& to_questions) {
		current_user.ResetToQuestions(to_questions);
	}

	void ResetFromQuestions(const std::vector<std::pair<int, int>>& to_questions) {
		current_user.ResetFromQuestions(to_questions);
	}

	void ListUsersNamesIds() const {
		for (const auto& pair : userame_userobject_map)
			std::cout << "ID: " << pair.second.GetUserId() << "\t\tName: " << pair.second.GetName() << "\n";
	}

	std::pair<int, int> ReadUserId() const {
		int user_id;
		std::cout << "Enter User id or -1 to cancel: ";
		std::cin >> user_id;

		if (user_id == -1)
			return std::make_pair(-1, -1);

		for (const auto& pair : userame_userobject_map) {
			if (pair.second.GetUserId() == user_id)
				return std::make_pair(user_id, pair.second.IsAllowAnonymousQuestions());
		}

		std::cout << "Invalid User ID. Try again\n";
		return ReadUserId();
	}

	void UpdateDatabase(const User& user) {
		std::string line = user.ToString();
		std::vector<std::string> lines(1, line);
		writeFileLines("users.txt", lines);
	}

	const User& GetCurrentUser() const {
		return current_user;
	}
};

class AskMeSystem {
private:
	UsersManager users_manager;
	QuestionsManager questions_manager;

	void LoadDatabase(bool fill_user_questions = false) {	// Internal
		users_manager.LoadDatabase();
		questions_manager.LoadDatabase();

		if (fill_user_questions)	// first time, waiting for login
			ResetCurrentUserQuestions();
	}

	/*
	 * Probably the most important design change
	 * We needed to decouple the question manager from relying on User implementation
	 * We break to 2 steps
	 * 1) Question manager return relevant question
	 * 2) User manager helps reseting the user question. Even the manager doesn't do this by itself
	 *
	 * This is more OO now
	 */
	void ResetCurrentUserQuestions() {
		const User& user = users_manager.GetCurrentUser();

		const auto& to_questions = questions_manager.GetUserToQuestions(user);
		users_manager.ResetToQuestions(to_questions);

		const auto& from_questions = questions_manager.GetUserFromQuestions(user);
		users_manager.ResetFromQuestions(from_questions);
	}

public:
	void Run() {	// only public one
		LoadDatabase(false);
		users_manager.AccessSystem();
		ResetCurrentUserQuestions();

		std::vector<std::string> menu;
		menu.push_back("Print Questions To Me");
		menu.push_back("Print Questions From Me");
		menu.push_back("Answer Question");
		menu.push_back("Delete Question");
		menu.push_back("Ask Question");
		menu.push_back("List System Users");
		menu.push_back("Feed");
		menu.push_back("Logout");

		while (true) {
			int choice = showReadMenu(menu);
			LoadDatabase(true);

			if (choice == 1)
				questions_manager.PrintUserToQuestions(users_manager.GetCurrentUser());
			else if (choice == 2)
				questions_manager.PrintUserFromQuestions(users_manager.GetCurrentUser());
			else if (choice == 3) {
				questions_manager.AnswerQuestion(users_manager.GetCurrentUser());
				questions_manager.UpdateDatabase();
			}
			else if (choice == 4) {
				questions_manager.DeleteQuestion(users_manager.GetCurrentUser());
				// Let's build again (just easier, but slow)
				ResetCurrentUserQuestions();
				questions_manager.UpdateDatabase();
			}
			else if (choice == 5) {
				std::pair<int, int> to_user_pair = users_manager.ReadUserId();
				if (to_user_pair.first != -1) {
					questions_manager.AskQuestion(users_manager.GetCurrentUser(), to_user_pair);
					questions_manager.UpdateDatabase();
				}
			}
			else if (choice == 6)
				users_manager.ListUsersNamesIds();
			else if (choice == 7)
				questions_manager.ListFeed();
			else
				break;
		}
		Run();	// Restart again
	}
};

int main() {
	AskMeSystem service;
	service.Run();

	return 0;
}
