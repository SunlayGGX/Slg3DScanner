#include "SlgSurfaceComputationPCH.h"
#include "CloudMerger.h"

#include "CloudVertex.h"


namespace Slg3DScanner
{
    namespace Details
    {
        class CloudMergerImpl
        {
        private:
            InternalCloudMesh& m_cloudTo;
            InternalCloudMesh m_cloudFrom;
            
            float m_eliminationDistance;

        public:
            CloudMergerImpl(InternalCloudMesh& to, InternalCloudMesh&& from) :
                m_cloudTo{ to },
                m_cloudFrom{ std::forward<InternalCloudMesh>(from) }
            {}

        public:
            void compute()
            {

            }

            bool searchProximity() const
            {
                bool insideLevelMargin = false;

                const auto endIter = m_cloudFrom.m_vertexes.end();
            }
        };
    }
}


using namespace Slg3DScanner;

void CloudMerger::merge(InternalCloudMesh& to, InternalCloudMesh&& from)
{
    Details::CloudMergerImpl{ to, std::forward<InternalCloudMesh>(from) }.compute();
}
