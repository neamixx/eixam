function composing():
    job.send("main.cpp")
end

function action()
    job.bash("g++ main.cpp -o main")
    job.send("main")
end

function result()
    job.bash("cat ./main")
end
