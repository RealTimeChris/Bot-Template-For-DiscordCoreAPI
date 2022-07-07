// HelperFunctions.hpp - Header for some helper functions.
// May 28, 2021
// Chris M.
// https://github.com/RealTimeChris

#pragma once

#include <discordcoreapi/Index.hpp>

namespace DiscordCoreAPI {

	float applyAsymptoticTransform(float inputModValue, float horizontalStretch, float ceiling) {
		float finalModValue = 0;
		float newInputModValue = inputModValue;
		if (newInputModValue == 0) {
			newInputModValue += 1;
		}
		if (newInputModValue <= 0) {
			float newInputValue = newInputModValue * -1;

			finalModValue = -1 * ( float )trunc((ceiling * pow(newInputValue, 3)) / ((pow(newInputValue, 3) + ( int64_t )horizontalStretch * ( int64_t )newInputValue)));
			return finalModValue;
		}

		finalModValue = ( float )trunc((ceiling * pow(newInputModValue, 3)) / ((pow(newInputModValue, 3) + ( int64_t )horizontalStretch * ( int64_t )newInputModValue)));

		return finalModValue;
	}

}// namespace DiscordCoreAPI
