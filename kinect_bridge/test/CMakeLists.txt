

add_executable(test_cvmatserialization
	test_cvmatserialization.cpp
	../cvmat_serialization.h
)

target_link_libraries(test_cvmatserialization
	${Boost_LIBRARIES}
	nestk
)
