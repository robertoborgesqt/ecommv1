#include "command.h"

using namespace Redis;

Command::Command(QString commandName, QObject *parent) : QObject(parent),
    commandName(commandName)
{
    arguments << commandName;
}

void Command::addArgument(QVariant argument)
{
    arguments << argument;
}

void Command::addArgumentStrings(QStringList arguments)
{
    foreach (QString argument, arguments) {
        addArgument(argument);
    }
}

QString Command::getCommandName()
{
    return arguments[0].toString();
}

QVariantList Command::getArguments()
{
    return arguments;
}

Command *Command::HGET(QString key, QString field)
{
    Command *command = new Command("HGET");

    command->addArgument(key);
    command->addArgument(field);

    return command;
}

Command *Command::GET(QString key)
{
    Command *command = new Command("GET");

    command->addArgument(key);

    return command;
}

Command *Command::DEL(QStringList keys)
{
    Command *command = new Command("DEL");

    command->addArgumentStrings(keys);

    return command;
}

Command *Command::EXPIRE(QString key, int seconds)
{
    Command *command = new Command("EXPIRE");

    command->addArgument(key);
    command->addArgument(seconds);

    return command;
}

Command *Command::KEYS(QString pattern)
{
    Command *command = new Command("KEYS");

    command->addArgument(pattern);

    return command;
}

Command *Command::EVAL(QString pattern)
{
	Command *command = new Command("EVAL");

	command->addArgument(pattern);

	return command;
}

Command *Command::LRANGE(QString key, QStringList values)
{
	Command *command = new Command("LRANGE");

	command->addArgument(key);
	command->addArgumentStrings(values); 

	return command;
}

Command *Command::LPUSH(QString key, QStringList values)
{
    Command *command = new Command("LPUSH");

    command->addArgument(key);
    command->addArgumentStrings(values);

    return command;
}

Command *Command::SET(QString key, QStringList values)
{
	Command *command = new Command("SET");

	command->addArgument(key);
	command->addArgumentStrings(values);

	return command;
}
Command *Command::RPOP(QString key)
{
    Command *command = new Command("RPOP");

    command->addArgument(key);

    return command;
}

Command *Command::SUBSCRIBE(QStringList channels)
{
    Command *command = new Command("SUBSCRIBE");

    command->addArgumentStrings(channels);

    return command;
}
